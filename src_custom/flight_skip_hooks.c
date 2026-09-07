#include "global.h"
#include "runtime.h"
#include "ram_map.h"
#include "level_up.h"
#include "data_structures.h"
#include "suction.h"
#include "nocash.h"
#include "debug_menu.h"
#include "overworld_encounters.h"

#define MAX_BOMBS 7

#define PLAYER_STATE_OFF 8
#define PLAYER_STATE_HURT 6
#define PLAYER_STATE_FLY 7
#define PLAYER_HP_OFF 0x34
#define PLAYER_FLAGS_OFF 0x18
#define PLAYER_HIT_FLAG 0x1000
#define PLAYER_STAGE_TYPE_OFF 0x22 /* see suction_hooks.c FullShipHpForPlayer */

/* Game-over mode queued by whatever vanilla collision handler kills the
 * ship — confirmed live via mGBA: a savestate hit from a specific attack
 * flipped gActorPool[0]'s state byte to 0 and QueueModeFade(0x98) in the
 * SAME frame, ~124 frames before gMode itself updates (the fade delay).
 * gActorPool[0]+0x34 (HP) was untouched at that moment — brute-force
 * pinning it every frame did not stop the transition — so this specific
 * kill path bypasses the HP scalar entirely and must be vetoed here. */
#define MODE_GAME_OVER 0x98

#define CANNON_ID_LO 1
#define CANNON_ID_HI 28
#define BULLET_ID_LO 29
#define BULLET_ID_HI 48
#define IMPACT_ID_LO 49
#define IMPACT_ID_HI 76 /* vanilla max; customs unlocked via EnsureCustomImpactsOwned */

/* CB "Have All Items" writes 0xFFFF to 0x03007740 and 0x03007742. */
#define ALL_ITEMS_MASK ((u32)0xFFFFFFFFu)

/*
 * CB "Have All Tools" slide: halfword 0xFFFF at gEventFlags+{2,6,10,14,18}.
 * That covers tool flags 31/59/88/118/155 and flag 24 (required by 0x1442C
 * before L/R tool cycling is allowed).
 */
static const u8 sToolFlagHalfwordOffs[] = { 2, 6, 10, 14, 18 };

/* Vanilla UpdateShooterFrame @ 0x08014E70: bl 0x14A48; bl 0x1749C */
typedef void (*VoidFunc)(void);

void OverworldPlayerUpdate__Continue(void);

static void ApplyToolsAndItems(void)
{
    u8 i;

    if (DebugToggle_AllItemsEnabled())
    {
        gItemsOwned = ALL_ITEMS_MASK;
        for (i = 0; i < sizeof(sToolFlagHalfwordOffs); i++)
            *(u16 *)(gEventFlags + sToolFlagHalfwordOffs[i]) = 0xFFFF;
    }
}

static void SetGunDataBits(int idLo, int idHi)
{
    int id;

    for (id = idLo; id <= idHi; id++)
        gGunDataBits[id >> 5] |= ((u32)1 << (id & 31));
}

#define CANNON_OWNED_MASK 0x3FFFFFFFu /* vanilla 28 + AUTO TARGET + EQUALIZER */
#define BULLET_OWNED_MASK 0x000FFFFFu /* vanilla 20 pieces */
#define IMPACT_OWNED_MASK 0x0FFFFFFFu /* vanilla 28 pieces */

static bool8 GunDataCheatsSatisfied(void)
{
    if (gRuntimeConfig.all_cannon_data && (gCannonOwned & CANNON_OWNED_MASK) != CANNON_OWNED_MASK)
        return FALSE;
    if (gRuntimeConfig.all_bullet_data && (gBulletOwned & BULLET_OWNED_MASK) != BULLET_OWNED_MASK)
        return FALSE;
    if (gRuntimeConfig.all_impact_data && (gImpactOwned & IMPACT_OWNED_MASK) != IMPACT_OWNED_MASK)
        return FALSE;
    return TRUE;
}

/* Gun-data unlocks are idempotent OR-writes. Latch skips repeat work, but
 * save loads can clear ownership while the latch stays set — re-apply when the
 * expected masks are incomplete. Status screen calls this via IsGunDataOwned. */
APPEND_TEXT void ApplyGunDataCheats(void)
{
    if (gInventoryCheatsApplied && GunDataCheatsSatisfied())
        return;
    gInventoryCheatsApplied = 1;

    if (gRuntimeConfig.all_cannon_data)
    {
        gCannonOwned |= CANNON_OWNED_MASK;
        SetGunDataBits(CANNON_ID_LO, CANNON_ID_HI);
    }

    if (gRuntimeConfig.all_bullet_data)
    {
        gBulletOwned |= BULLET_OWNED_MASK;
        SetGunDataBits(BULLET_ID_LO, BULLET_ID_HI);
    }

    if (gRuntimeConfig.all_impact_data)
    {
        gImpactOwned |= IMPACT_OWNED_MASK;
        SetGunDataBits(IMPACT_ID_LO, IMPACT_ID_HI);
    }

    /* Append pieces from impact / cannon / bullet JSON. */
    EnsureCustomImpactsOwned();
    EnsureCustomCannonsOwned();
    EnsureCustomBulletsOwned();
}

/* 0x03007080 is the flight HUD quota / enemy-remaining digit (vanilla writes
 * 6, 13, 30, …) — not ship HP.
 *
 * gPlayerPtr is NOT reliable here: confirmed via mGBA live probe that in the
 * Ch.1-opener-style flight stage (gMode 0x84) gPlayerPtr's target reads all
 * zero for thousands of frames — flight ships live in gActorPool[0] instead
 * (see ram_map.h: "flight [0]=ship"; gActorPool[0]'s position/velocity track
 * the ship every frame, confirmed live). Use the actor-pool slot, matching
 * the veto in overworld_enemy_exp_hooks.c's DamageApply__Replacement.
 *
 * Also confirmed live: in that same stage, player+0x34 never carries a real
 * HP value (stays 0 through death) — the visible HP bar there is animated
 * directly via soft-OAM sprite geometry with no separate RAM-resident HP
 * counter found after a full IWRAM/EWRAM diff across multiple confirmed
 * hits, so this pin cannot stop that stage's damage on its own; it is kept
 * so a lethal write to +0x34 doesn't stick if that stage variant, a boss
 * fight, or a future stage type does honor it (matches suction.h
 * FullShipHpForPlayer / PLAYER_HP_OFF, shared with the Phoenix revive path).
 * Pinning only the hurt *state* left HP free to drain to 0 and still trigger
 * the lethal/DeathFx path even though the state kept getting nudged back to
 * flying — force HP to full and clear the hit flag every frame instead. */
static void ApplyMaxHealth(void)
{
    u8 *player = &gActorPool[0];

    *(u32 *)(player + PLAYER_HP_OFF) = FullShipHpForPlayer(player);
    *(u16 *)(player + PLAYER_FLAGS_OFF) &= (u16)~PLAYER_HIT_FLAG;

    if (player[PLAYER_STATE_OFF] == PLAYER_STATE_HURT)
        player[PLAYER_STATE_OFF] = PLAYER_STATE_FLY;
}

void QueueModeFade__Continue(u32 modeId, u32 fadeSpeed);

/* QueueModeFade @ 0x0800D734 — veto only the game-over transition while Max
 * Health is enabled; every other caller (warps, status panel, boss/story
 * mode changes) passes through untouched. Vetoing here, not the HP write
 * that would normally accompany it, is required: this kill path was
 * confirmed live to queue mode 0x98 in the same frame gActorPool[0]'s state
 * goes lethal, without ever writing a losing value to +0x34 first. */
APPEND_TEXT void QueueModeFade__Replacement(u32 modeId, u32 fadeSpeed)
{
    if ((modeId & 0xFF) == MODE_GAME_OVER && DebugToggle_MaxHealthEnabled())
        return;

    QueueModeFade__Continue(modeId, fadeSpeed);
}

static u8 ConfiguredMaxLevel(void)
{
    return gRuntimeConfig.level_cap_255 ? MAX_PLAYER_LEVEL_EXTENDED
                                        : MAX_PLAYER_LEVEL;
}

/* Bump to the active cap once per low-level state (save loads re-trigger). */
static void ApplyMaxLevel(void)
{
    u8 maxLevel;

    if (!gRuntimeConfig.start_max_level)
        return;

    maxLevel = ConfiguredMaxLevel();
    if (gPlayerLevel == maxLevel)
        return;

    gPlayerLevel = maxLevel;
    CalcExpToNextLevel();
    gPlayerExp = gExpToNextLevel;
}

/* Overworld walk update @ 0x0801DC84 — apply unlocks before flight stages. */
APPEND_TEXT void OverworldPlayerUpdate__Replacement(void)
{
    if (DebugMenu_IsBlocking())
        return;

    ApplyMaxLevel();
    ApplyToolsAndItems();
    ApplyGunDataCheats();
    OverworldPlayerUpdate__Continue();
}

APPEND_TEXT void UpdateShooterFrame__Replacement(void)
{
    /* One-shot so No$GBA TTY can be verified before any death path.
     * Latch is free-pool IWRAM — C static would land in .bss @ 0x03000000. */
    if (!gNoCashHeartbeat)
    {
        gNoCashHeartbeat = 1;
        NoCashGBAPrint("PHX nocash heartbeat: UpdateShooterFrame");
    }

    /* Vanilla meter / kill credit first, then light refills. */
    ((VoidFunc)0x08014A49)();
    ((VoidFunc)0x0801749D)();

    if (DebugToggle_MaxHealthEnabled())
        ApplyMaxHealth();

    if (DebugToggle_MaxBombsEnabled() && gPlayerBombs < MAX_BOMBS)
        gPlayerBombs = MAX_BOMBS;

    ApplyMaxLevel();
    ApplyPhoenixRevive();
    TickPhoenixRevivePopup();
    ApplyAutoTarget();
    ApplyEqualizer();
    ApplyLaserBeam();
    ApplyChargeShot();
    ApplyToolsAndItems();
    ApplyGunDataCheats();
}

/* Veneered over AddExperience @ 0x0800FDC4 when exp_multiplier != 1,
 * level_cap_255, and/or custom_gun_data (Training Weights EXP).
 * Must match vanilla: only mutate gPlayerExp / level.
 * Do NOT write gPlayerExpDisplay — HudSync copies + RebuildExpDigits when they differ. */
APPEND_TEXT static u32 ScaleExperienceAmount(u32 amount)
{
    u32 mult = gRuntimeConfig.exp_multiplier;
    u32 scaled;

    if (mult == 0)
        mult = 1;

    scaled = amount * mult;

    if (gRuntimeConfig.custom_gun_data && TrainingWeightsIsEquipped())
        scaled += scaled / 2;

    return scaled;
}

APPEND_TEXT bool8 AddExperience__Replacement(u32 amount)
{
    u8 level = gPlayerLevel;
    u8 maxLevel = ConfiguredMaxLevel();

    if (level > maxLevel - 1)
        return FALSE;

    amount = ScaleExperienceAmount(amount);

    gPlayerExp += amount;

    if (level == maxLevel)
    {
        if (gPlayerExp > gExpToNextLevel)
            gPlayerExp = gExpToNextLevel;
    }

    if (gPlayerExp >= gExpToNextLevel)
    {
        gPlayerLevel++;
        if (gPlayerLevel > maxLevel - 1)
            gPlayerLevel = maxLevel;
        CalcExpToNextLevel();
        return TRUE;
    }

    return FALSE;
}

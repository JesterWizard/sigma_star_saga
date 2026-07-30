#include "global.h"
#include "runtime.h"
#include "ram_map.h"
#include "level_up.h"
#include "data_structures.h"
#include "suction.h"
#include "nocash.h"
#include "debug_menu.h"

#define MAX_BOMBS 7

#define PLAYER_STATE_OFF 8
#define PLAYER_STATE_HURT 6
#define PLAYER_STATE_FLY 7

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

    if (gRuntimeConfig.all_key_items)
        gItemsOwned = ALL_ITEMS_MASK;

    if (gRuntimeConfig.all_tools)
    {
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
 * 6, 13, 30, …) — not ship HP. Infinite health cancels the hurt state instead. */
static void ApplyMaxHealth(void)
{
    u8 *player = gPlayerPtr;

    if (player != NULL && player[PLAYER_STATE_OFF] == PLAYER_STATE_HURT)
        player[PLAYER_STATE_OFF] = PLAYER_STATE_FLY;
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

    if (gRuntimeConfig.always_max_health)
        ApplyMaxHealth();

    if (gRuntimeConfig.always_max_bombs && gPlayerBombs < MAX_BOMBS)
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

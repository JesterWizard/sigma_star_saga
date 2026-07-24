#include "global.h"
#include "runtime.h"
#include "ram_map.h"
#include "actor.h"
#include "data_structures.h"
#include "suction.h"
#include "nocash.h"

typedef u32 (*CalcAngleFn)(s32 x0, s32 y0, s32 x1, s32 y1);

#define CALC_ANGLE ((CalcAngleFn)0x08003D15)
#define SIN_TABLE ((s16 *)0x0805A93C)

/* Per-type Gun Data piece counts {cannon, bullet, impact, item}; the cannon and
 * impact words are widened by apply_lynjump.py for the custom pieces. */
#define GUN_DATA_TYPE_COUNTS ((const u32 *)0x080F0A98)
#define GUN_DATA_TYPE_MAX 4

/* Sin table amplitude is 256, so one frame bends a shot by up to 1 pixel. */
/* Position nudge per frame (sintab << shift). 10 ≈ 4× the old shift-8 pull. */
#define AUTO_TARGET_BEND_SHIFT 10
/* Actor velocity (16.16); x += vx / y += vy in vanilla integrate. */
#define ACTOR_OFF_VX 0x48
#define ACTOR_OFF_VY 0x54

#define SHOOTER_FILE_TABLE 0x086188C4
#define GUN_ICON_FILE_INDEX 231 /* 1-based shooter archive index for ANM #230 */
#define EXP_POPUP_FILE_INDEX 0x71 /* 1-based; green "+ EXP" floating text ANM */
#define CUR_ARCHIVE_FT_PTR ((volatile u32 *)0x03001200)

/* Vanilla SpawnExpText @ 0x0804AA48 — floats green pickup label at (x, y). */
typedef void (*SpawnExpTextFn)(s32 x, s32 y);
#define SPAWN_EXP_TEXT ((SpawnExpTextFn)0x0804AA49)

/* Re-DMA every loaded shooter ANM bank from GetArchiveFileStart sources. */
typedef void (*ReloadAnmBanksFn)(void);
#define RELOAD_ANM_BANKS ((ReloadAnmBanksFn)0x08005E7D)

/* Defined in asm/suction_trampoline.s */
void ExpGemUpdate__Continue(u32 arg);
void LeechGemUpdate__Continue(void);
void GetGunDataIconFrame__Continue(u32 type, u32 local, u32 owned, u16 *out);
u32 IsGunDataOwned__Continue(u32 type, u32 local);

/*
 * Green "REVIVE" popup — same ANM layout as shooter file 0x71 ("+ EXP").
 * Swapped in via GetArchiveFileStart while gPhoenixReviveAnmSwap is set.
 */
APPEND_RODATA static const u8 sRevivePopupAnm[188] = {
    0x00, 0x00, 0x01, 0x00, 0x80, 0x00, 0x01, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0xF7, 0xFF, 0xF7, 0xFF,
    0xF7, 0xFF, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0xFD, 0xFF, 0xFD, 0xFF, 0xFD, 0xFF, 0x03, 0x00,
    0x03, 0x00, 0x03, 0x00, 0x01, 0x00, 0xF1, 0xFF, 0xFD, 0xFF, 0x00, 0x14, 0x1A, 0x11, 0xAA, 0x11,
    0x1A, 0xAA, 0xA1, 0xA1, 0x1A, 0x11, 0xAA, 0x11, 0x1A, 0x1A, 0xAA, 0xA1, 0x1A, 0xAA, 0xA1, 0xA1,
    0x1A, 0xAA, 0xA1, 0x11, 0xAA, 0xAA, 0xAA, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x11, 0x1A, 0xAA, 0xA1,
    0xAA, 0x1A, 0xAA, 0xA1, 0xA1, 0x1A, 0xAA, 0xA1, 0xAA, 0xAA, 0xA1, 0xA1, 0xAA, 0xAA, 0xA1, 0xA1,
    0x11, 0xAA, 0x1A, 0xAA, 0xAA, 0x0A, 0xAA, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x1A, 0x0A, 0x1A, 0xAA,
    0x1A, 0x0A, 0x1A, 0xAA, 0x1A, 0x0A, 0x1A, 0xAA, 0x1A, 0x0A, 0xAA, 0xA1, 0x1A, 0x0A, 0xA0, 0xA1,
    0x1A, 0x0A, 0xA0, 0x1A, 0xAA, 0x0A, 0x00, 0xAA, 0x00, 0x00, 0x00, 0x00, 0xA1, 0x11, 0x11, 0x0A,
    0xA1, 0xA1, 0xAA, 0x0A, 0xA1, 0x11, 0xA1, 0x00, 0xA1, 0xA1, 0xAA, 0x00, 0xA1, 0xA1, 0xAA, 0x0A,
    0xAA, 0x11, 0x11, 0x0A, 0xAA, 0xAA, 0xAA, 0x0A, 0x00, 0x00, 0x00, 0x00,
};

#define PLAYER_STATE_OFF 8
#define PLAYER_STATE_SPAWN 1
#define PLAYER_STATE_HURT 6
#define PLAYER_STATE_FLY 7
#define PLAYER_STATE_DYING 0x2F /* HitUpdate lethal write @ 0x08025232 */
#define PLAYER_STATE_DEAD_A 0x30
#define PLAYER_HP_OFF 0x34
#define PLAYER_IFRAME_OFF 0x12
#define PLAYER_ANIM_TIMER_OFF 0x2C
#define PLAYER_STAGE_TYPE_OFF 0x22
#define PLAYER_FLAGS_OFF 0x18
#define PLAYER_HIT_FLAG 0x1000
#define PHOENIX_MAGIC_A 0xA5
#define PHOENIX_MAGIC_B 0x5A

APPEND_TEXT u32 EquippedImpactIndex(void)
{
    u32 idx;

    if (gGunLoadoutPrimaryFlag != 0)
        idx = gGunLoadoutImpact;
    else
        idx = gGunLoadoutImpactAlt;
    return idx & 0xFF;
}

APPEND_TEXT u32 ImpactIdIsPhoenix(u32 id)
{
    id &= 0xFF;
    return id == IMPACT_PHOENIX
        || id == IMPACT_ID_PHOENIX
        || id == IMPACT_NUM_PHOENIX;
}

APPEND_TEXT u32 PhoenixIsEquipped(void)
{
    u32 idx = EquippedImpactIndex();
    u32 primary = gGunLoadoutImpact & 0xFF;
    u32 alt = gGunLoadoutImpactAlt & 0xFF;

    /* Active slot first (same as OnImpact / Suction). Also accept the other
     * loadout word so a sticky primary-flag mismatch cannot soft-disable it.
     * Phoenix may appear as local index 30, Gun Data ID 79, or badge #31.
     * Do NOT treat mere ownership as equipped — all_impact_data would make
     * Phoenix always-on and poison DeleteActor skips on the overworld. */
    if (ImpactIdIsPhoenix(idx) || ImpactIdIsPhoenix(primary) || ImpactIdIsPhoenix(alt))
        return 1;

    return 0;
}

/* Stage-type HP stock matches vanilla init @ 0x08021338. */
APPEND_TEXT u32 FullShipHpForPlayer(u8 *player)
{
    u16 stage = *(u16 *)(player + PLAYER_STAGE_TYPE_OFF);

    if (stage == 2)
        return 3;
    if (stage == 0x42)
        return 4;
    return 5;
}

APPEND_TEXT void EnsurePhoenixReviveState(u8 *player)
{
    if (player == NULL)
        return;

    if (gPhoenixReviveMagicA != PHOENIX_MAGIC_A
        || gPhoenixReviveMagicB != PHOENIX_MAGIC_B
        || player[PLAYER_STATE_OFF] == PLAYER_STATE_SPAWN)
    {
        gPhoenixReviveUsed = 0;
        gPhoenixReviveTailSkips = 0;
        gPhoenixReviveMagicA = PHOENIX_MAGIC_A;
        gPhoenixReviveMagicB = PHOENIX_MAGIC_B;
    }
}

APPEND_TEXT u32 PlayerNeedsPhoenixRevive(u8 *player)
{
    u8 state = player[PLAYER_STATE_OFF];
    u32 hp = *(u32 *)(player + PLAYER_HP_OFF);
    u16 flags = *(u16 *)(player + PLAYER_FLAGS_OFF);
    u32 hpGone = (hp == 0 || hp == 0xFF);

    /* Only true lethal signals. Do NOT treat living special-form states
     * (0x13 / 0x34) or generic HURT+hit-flag as death — that consumed the
     * one revive before HP actually hit 0. */
    if (state == PLAYER_STATE_DYING)
        return 1;
    if (state == PLAYER_STATE_DEAD_A && hpGone)
        return 1;

    /* ShipUpdate @ 0x16114 / stage DeathFx callers: hit flag + empty HP. */
    if ((flags & PLAYER_HIT_FLAG) != 0 && hpGone)
        return 1;

    return 0;
}

/* True when the ship looks like it is on a lethal / crash path (for logs). */
APPEND_TEXT u32 PlayerLooksCrashed(u8 *player)
{
    if (player == NULL)
        return 0;
    return PlayerNeedsPhoenixRevive(player);
}

APPEND_TEXT void LogPhoenixCrashProbe(const char *tag)
{
    u8 *player = gPlayerPtr;
    u32 state = 0xEE;
    u32 hp = 0xEEEEEEEE;
    u32 flags = 0;
    u32 primary = gGunLoadoutImpact & 0xFF;
    u32 alt = gGunLoadoutImpactAlt & 0xFF;
    u32 active = EquippedImpactIndex();
    u32 slotFlag = gGunLoadoutPrimaryFlag & 0xFF;

    if (player != NULL)
    {
        state = player[PLAYER_STATE_OFF];
        hp = *(u32 *)(player + PLAYER_HP_OFF);
        flags = *(u16 *)(player + PLAYER_FLAGS_OFF);
    }

    NoCashGBAPrintf(
        "PHX %s st=%u hp=%u fl=%x eq=%u used=%u act=%u pri=%u alt=%u sf=%u need=%u",
        tag,
        state,
        hp,
        flags,
        PhoenixIsEquipped(),
        gPhoenixReviveUsed & 0xFF,
        active,
        primary,
        alt,
        slotFlag,
        (player != NULL) ? PlayerNeedsPhoenixRevive(player) : 0);
}

/* Called from PlayerDeathFx veneer before revive attempt (once per death FX). */
APPEND_TEXT void LogPhoenixDeathFxEntry(void)
{
    if (gPhoenixCrashLogged)
        return;
    NoCashGBAPrint("PHX DeathFx ENTRY");
    LogPhoenixCrashProbe("DeathFx");
    /* Leave latch clear so DoPhoenixRevive still prints its decision. */
}

APPEND_TEXT void ClearPhoenixCrashLogLatch(void)
{
    gPhoenixCrashLogged = 0;
}

/* Perform revive. No dying-state gate — callers decide when.
 * Do not arm gPhoenixReviveTailSkips: every known PlayerDeathFx caller
 * already jumps past DeleteActor via PhoenixDeathFxSkipReturn. A leftover
 * skip was softlocking overworld (save plug / rain / empty dialogue). */
APPEND_TEXT u32 DoPhoenixRevive(void)
{
    u8 *player = gPlayerPtr;
    u32 log = !gPhoenixCrashLogged;

    /* At most one TTY dump per crash (Apply may call this every frame). */
    if (log)
    {
        gPhoenixCrashLogged = 1;
        NoCashGBAPrint("PHX DoRevive enter");
        LogPhoenixCrashProbe("DoRevive");
    }

    if (!gRuntimeConfig.custom_gun_data)
    {
        if (log)
            NoCashGBAPrint("PHX reject: custom off");
        return 0;
    }
    if (player == NULL)
    {
        if (log)
            NoCashGBAPrint("PHX reject: no player");
        return 0;
    }
    if (!PhoenixIsEquipped())
    {
        if (log)
            NoCashGBAPrint("PHX reject: not equipped");
        return 0;
    }
    EnsurePhoenixReviveState(player);
    if (player[PLAYER_STATE_OFF] == PLAYER_STATE_SPAWN)
    {
        if (log)
            NoCashGBAPrint("PHX reject: spawn state");
        return 0;
    }
    if (gPhoenixReviveUsed)
    {
        if (log)
            NoCashGBAPrint("PHX reject: already used");
        return 0;
    }

    NoCashGBAPrint("PHX revive: SUCCESS");
    gPhoenixReviveUsed = 1;
    gPhoenixReviveTailSkips = 0;
    *(u32 *)(player + PLAYER_HP_OFF) = FullShipHpForPlayer(player);
    player[PLAYER_STATE_OFF] = PLAYER_STATE_FLY;
    player[PLAYER_IFRAME_OFF] = 0x3C;
    player[0x11] = 0xFF;
    *(s16 *)(player + PLAYER_ANIM_TIMER_OFF) = 0;
    *(u16 *)(player + PLAYER_FLAGS_OFF) &= (u16)~PLAYER_HIT_FLAG;
    /* Clear velocity so a death-zeroed ship doesn't softlock mid-explode. */
    *(u32 *)(player + 0x48) = 0;
    *(u32 *)(player + 0x54) = 0;
    /* Popup after HP restore — must not block the skip-death return path. */
    SpawnPhoenixRevivePopup();
    return 1;
}

/*
 * DeathFx callers often still fire after ShipUpdate already revived this frame.
 * Skipping only on DoPhoenixRevive success then hits "already used" and plays
 * explosion → DeleteActor → game over. Treat a healthy post-revive ship as skip.
 */
APPEND_TEXT u32 PhoenixDeathFxShouldSkip(void)
{
    u8 *player;

    if (DoPhoenixRevive())
        return 1;

    if (!gPhoenixReviveUsed)
        return 0;

    player = gPlayerPtr;
    if (player == NULL)
        return 0;
    if (PlayerNeedsPhoenixRevive(player))
        return 0;

    NoCashGBAPrint("PHX DeathFx skip: already revived");
    return 1;
}

/* Float green "REVIVE" at the ship — same path as "+ EXP" gem pickup text.
 * Swap only for the spawn GetArchiveFileStart(0x71); do NOT RELOAD_ANM_BANKS
 * here (full-bank DMA during death can corrupt state / lose the skip). */
APPEND_TEXT void SpawnPhoenixRevivePopup(void)
{
    u8 *player = gPlayerPtr;
    s32 x;
    s32 y;

    if (player == NULL)
        return;

    x = *(s32 *)(player + 0x40);
    y = *(s32 *)(player + 0x4C);

    gPhoenixReviveAnmSwap = 1;
    SPAWN_EXP_TEXT(x, y);
    gPhoenixReviveAnmSwap = 0;
    /* Restore "+ EXP" bank after the float finishes (Tick does the reload). */
    gPhoenixReviveAnmRestore = 60;
}

APPEND_TEXT void TickPhoenixRevivePopup(void)
{
    if (gPhoenixReviveAnmRestore == 0)
        return;
    gPhoenixReviveAnmRestore--;
    if (gPhoenixReviveAnmRestore != 0)
        return;
    gPhoenixReviveAnmSwap = 0;
    RELOAD_ANM_BANKS();
}

/* Called from hit-update / state-machine after death may have been written. */
APPEND_TEXT void ApplyPhoenixRevive(void)
{
    u8 *player = gPlayerPtr;

    if (!gRuntimeConfig.custom_gun_data || player == NULL)
        return;

    /* Edge-trigger latch for DeathFx/DoRevive TTY — clear when healthy. */
    if (!PlayerLooksCrashed(player))
        ClearPhoenixCrashLogLatch();

    if (!PhoenixIsEquipped())
        return;
    EnsurePhoenixReviveState(player);

    if (player[PLAYER_STATE_OFF] == PLAYER_STATE_SPAWN)
        return;

    /* Vanilla often sets HP to 0xFF right before state 0x2F; stage paths
     * leave HP at 0 instead. Accept either signal. */
    if (PlayerNeedsPhoenixRevive(player))
        DoPhoenixRevive();
}

/* 7 = Suction (vanilla magnet speed), 8 = Suction+ (2×), 0 = unequipped. */
APPEND_TEXT u32 EquippedSuctionSpeedShift(void)
{
    u32 idx = EquippedImpactIndex();

    if (idx == IMPACT_SUCTION)
        return 7;
    if (idx == IMPACT_SUCTION_PLUS)
        return 8;
    return 0;
}

/* Match vanilla gem magnet math (angle → sintab << shift), no range gate.
 * ExpGemUpdate tracks the gem in gExpGemPtr (+0x18/+0x1C); LeechGemUpdate
 * uses gPlayerPtr with actor-style coords (+0x40/+0x4C). */
APPEND_TEXT void AttractExpGem(u8 *gem, u32 pos_off, u32 speed_shift)
{
    u8 *player = gActorPool;
    s32 *gx;
    s32 *gy;
    s32 px;
    s32 py;
    u32 angle;

    if (gem == NULL || speed_shift == 0)
        return;

    gx = (s32 *)(gem + pos_off);
    gy = (s32 *)(gem + pos_off + 4);
    px = *(s32 *)(player + 0x40);
    py = *(s32 *)(player + 0x4C);
    angle = CALC_ANGLE(*gx, *gy, px, py) & 0xFF;
    *gx += ((s32)SIN_TABLE[angle + 64]) << speed_shift;
    *gy += ((s32)SIN_TABLE[angle]) << speed_shift;
}

APPEND_TEXT void ExpGemUpdate__Replacement(u32 arg)
{
    u32 shift = EquippedSuctionSpeedShift();

    if (shift != 0)
        AttractExpGem(gExpGemPtr, 0x18, shift);
    ExpGemUpdate__Continue(arg);
}

APPEND_TEXT void LeechGemUpdate__Replacement(void)
{
    u32 shift = EquippedSuctionSpeedShift();

    if (shift != 0)
        AttractExpGem(gPlayerPtr, 0x40, shift);
    LeechGemUpdate__Continue();
}

/* OnCannon @ 0x0802D76C reads the equipped cannon as a plain local index. */
APPEND_TEXT u32 EquippedCannonIndex(void)
{
    return gGunLoadout & 0xFF;
}

APPEND_TEXT u32 AutoTargetIsEquipped(void)
{
    return EquippedCannonIndex() == CANNON_AUTO_TARGET;
}

/* Nearest live enemy to (x, y), using the same actor class the vanilla
 * target search @ 0x0802EF1A matches. Distance is compared in whole pixels
 * so dx*dx + dy*dy cannot overflow the 16.16 world coords. */
APPEND_TEXT static u8 *NearestEnemyActor(s32 x, s32 y)
{
    u8 *best = NULL;
    s32 bestDist = 0x7FFFFFFF;
    u32 i;

    for (i = 1; i < ACTOR_COUNT; i++)
    {
        u8 *actor = &gActorPool[i * ACTOR_STRIDE];
        s32 dx;
        s32 dy;
        s32 dist;

        if ((*(u16 *)(actor + ACTOR_OFF_FLAGS) & ACTOR_FLAG_ACTIVE) == 0)
            continue;
        if (actor[ACTOR_OFF_CLASS] != ACTOR_CLASS_ENEMY)
            continue;
        if (*(u32 *)(actor + ACTOR_OFF_HP) == 0)
            continue;

        dx = (*(s32 *)(actor + ACTOR_OFF_X) >> 16) - (x >> 16);
        dy = (*(s32 *)(actor + ACTOR_OFF_Y) >> 16) - (y >> 16);
        dist = dx * dx + dy * dy;
        if (dist < bestDist)
        {
            bestDist = dist;
            best = actor;
        }
    }
    return best;
}

/*
 * AUTO TARGET (29th Cannon Data): bend live player shots toward the closest
 * enemy. Stronger than gem-magnet position nudge alone — also retargets vx/vy
 * so high/low angles are not drowned by forward speed. Fire rate from
 * fire_from in cannon_data.json (Rapid Cannon).
 */
APPEND_TEXT void ApplyAutoTarget(void)
{
    u32 i;

    if (!gRuntimeConfig.custom_gun_data || gPlayerPtr == NULL)
        return;
    if (!AutoTargetIsEquipped())
        return;

    for (i = 1; i < ACTOR_COUNT; i++)
    {
        u8 *shot = &gActorPool[i * ACTOR_STRIDE];
        u8 *target;
        s32 *sx;
        s32 *sy;
        s32 cosine;
        s32 sine;
        s32 vx;
        s32 vy;
        s32 speed;
        u32 angle;

        if ((*(u16 *)(shot + ACTOR_OFF_FLAGS) & ACTOR_FLAG_ACTIVE) == 0)
            continue;
        if (shot[ACTOR_OFF_CLASS] != ACTOR_CLASS_PLAYER_SHOT)
            continue;

        sx = (s32 *)(shot + ACTOR_OFF_X);
        sy = (s32 *)(shot + ACTOR_OFF_Y);
        target = NearestEnemyActor(*sx, *sy);
        if (target == NULL)
            return;

        angle = CALC_ANGLE(*sx, *sy,
                           *(s32 *)(target + ACTOR_OFF_X),
                           *(s32 *)(target + ACTOR_OFF_Y)) & 0xFF;
        cosine = (s32)SIN_TABLE[angle + 64];
        sine = (s32)SIN_TABLE[angle];
        *sx += cosine << AUTO_TARGET_BEND_SHIFT;
        *sy += sine << AUTO_TARGET_BEND_SHIFT;

        vx = *(s32 *)(shot + ACTOR_OFF_VX);
        vy = *(s32 *)(shot + ACTOR_OFF_VY);
        speed = (vx < 0 ? -vx : vx) + (vy < 0 ? -vy : vy);
        if (speed < 0x20000)
            speed = 0x40000;
        /* sintab ±0x100; (speed >> 8) keeps |vel| ≈ speed when aligned. */
        *(s32 *)(shot + ACTOR_OFF_VX) = cosine * (speed >> 8);
        *(s32 *)(shot + ACTOR_OFF_VY) = sine * (speed >> 8);
    }
}

/* Status UI ownership — unlock custom impacts before the ??? fallback. */
APPEND_TEXT u32 IsGunDataOwned__Replacement(u32 type, u32 local)
{
    EnsureCustomImpactsOwned();
    return IsGunDataOwned__Continue(type, local);
}

/* Redirect shooter gun-icon ANM to the extended blob with Suction frames.
 * Also swaps the "+ EXP" popup ANM for "REVIVE" while Phoenix is spawning it. */
APPEND_TEXT void *GetArchiveFileStart__Replacement(u32 index)
{
    u32 ft = *CUR_ARCHIVE_FT_PTR;
    u32 *table;

    if (ft == SHOOTER_FILE_TABLE && index == GUN_ICON_FILE_INDEX && gGunIconAnmExtSize != 0)
        return gGunIconAnmExt;

    if (ft == SHOOTER_FILE_TABLE && index == EXP_POPUP_FILE_INDEX && gPhoenixReviveAnmSwap)
        return (void *)sRevivePopupAnm;

    table = (u32 *)ft;
    return (void *)(ft + table[index]);
}

APPEND_TEXT u32 GetArchiveFileSize__Replacement(u32 index)
{
    u32 ft = *CUR_ARCHIVE_FT_PTR;
    u32 *entry;

    if (ft == SHOOTER_FILE_TABLE && index == GUN_ICON_FILE_INDEX && gGunIconAnmExtSize != 0)
        return gGunIconAnmExtSize;

    if (ft == SHOOTER_FILE_TABLE && index == EXP_POPUP_FILE_INDEX && gPhoenixReviveAnmSwap)
        return sizeof(sRevivePopupAnm);

    entry = (u32 *)(ft + index * 4);
    return entry[1] - entry[0];
}

/* Vanilla frame layout: cannon 0, bullet 28, impact 48, item 76 (× 2 frames). */
APPEND_RODATA static const u8 sGunIconTypeBase[GUN_DATA_TYPE_MAX] = { 0, 28, 48, 76 };

/*
 * Custom impacts use extended ANM frames 196+ (one locked/owned pair each),
 * custom cannons the pairs right after them. Vanilla pieces keep the Continue
 * path — except out-of-range locals, which vanilla would resolve to another
 * type's frames (an equipped index past the live count rendered as a red
 * bullet icon). Clamp those to the type's own first frame instead.
 */
APPEND_TEXT void GetGunDataIconFrame__Replacement(u32 type, u32 local, u32 owned, u16 *out)
{
    u32 kind = type & 0xFF;
    u16 i;

    EnsureCustomImpactsOwned();

    if (kind == 2)
    {
        for (i = 0; i < gCustomImpactCount; i++)
        {
            if (gCustomImpacts[i].index == (u8)local)
            {
                *out = (u16)(SUCTION_ICON_FRAME_BASE + i * 2
                             + ((owned & 0xFF) ? 1 : 0));
                return;
            }
        }
    }

    if (kind == 0)
    {
        for (i = 0; i < gCustomCannonCount; i++)
        {
            if (gCustomCannons[i].index == (u8)local)
            {
                *out = (u16)(SUCTION_ICON_FRAME_BASE + gCustomImpactCount * 2
                             + i * 2 + ((owned & 0xFF) ? 1 : 0));
                return;
            }
        }
    }

    if (kind < GUN_DATA_TYPE_MAX && (local & 0xFF) >= GUN_DATA_TYPE_COUNTS[kind])
    {
        *out = (u16)(sGunIconTypeBase[kind] * 2);
        return;
    }

    GetGunDataIconFrame__Continue(type, local, owned, out);
}

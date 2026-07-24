#include "global.h"
#include "runtime.h"
#include "ram_map.h"
#include "data_structures.h"
#include "suction.h"
#include "nocash.h"

typedef u32 (*CalcAngleFn)(s32 x0, s32 y0, s32 x1, s32 y1);

#define CALC_ANGLE ((CalcAngleFn)0x08003D15)
#define SIN_TABLE ((s16 *)0x0805A93C)

#define SHOOTER_FILE_TABLE 0x086188C4
#define GUN_ICON_FILE_INDEX 231 /* 1-based shooter archive index for ANM #230 */
#define CUR_ARCHIVE_FT_PTR ((volatile u32 *)0x03001200)

/* Defined in asm/suction_trampoline.s */
void ExpGemUpdate__Continue(u32 arg);
void LeechGemUpdate__Continue(void);
void GetGunDataIconFrame__Continue(u32 type, u32 local, u32 owned, u16 *out);
u32 IsGunDataOwned__Continue(u32 type, u32 local);

#define PLAYER_STATE_OFF 8
#define PLAYER_STATE_SPAWN 1
#define PLAYER_STATE_HURT 6
#define PLAYER_STATE_FLY 7
#define PLAYER_STATE_DYING 0x2F
#define PLAYER_STATE_DEAD_A 0x30
#define PLAYER_STATE_DEAD_B 9
#define PLAYER_STATE_DEAD_C 0x3A /* stage-handler lethal path @ 0x08023B68 */
#define PLAYER_STATE_DEAD_D 0x13 /* post-dying anim; lives drain @ PlayerHitUpdate */
#define PLAYER_STATE_DEAD_E 0x34 /* special-form lethal twin of 0x13 */
#define PLAYER_HP_OFF 0x34
#define PLAYER_IFRAME_OFF 0x12
#define PLAYER_ANIM_TIMER_OFF 0x2C
#define PLAYER_STAGE_TYPE_OFF 0x22
#define PLAYER_FLAGS_OFF 0x18
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

APPEND_TEXT u32 PhoenixIsEquipped(void)
{
    u32 idx = EquippedImpactIndex();
    u32 primary = gGunLoadoutImpact & 0xFF;
    u32 alt = gGunLoadoutImpactAlt & 0xFF;

    /* Active slot first (same as OnImpact / Suction). Also accept the other
     * loadout word so a sticky primary-flag mismatch cannot soft-disable it.
     * Custom Phoenix may appear as local index 30 or Gun Data ID 79.
     * Do NOT treat mere ownership as equipped — all_impact_data would make
     * Phoenix always-on and poison DeleteActor skips on the overworld. */
    if (idx == IMPACT_PHOENIX || idx == IMPACT_ID_PHOENIX)
        return 1;
    if (primary == IMPACT_PHOENIX || primary == IMPACT_ID_PHOENIX)
        return 1;
    if (alt == IMPACT_PHOENIX || alt == IMPACT_ID_PHOENIX)
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

    /* Dying / post-death states (vanilla sets these on the lethal path). */
    if (state == PLAYER_STATE_DYING
        || state == PLAYER_STATE_DEAD_A
        || state == PLAYER_STATE_DEAD_B
        || state == PLAYER_STATE_DEAD_C
        || state == PLAYER_STATE_DEAD_D
        || state == PLAYER_STATE_DEAD_E)
        return 1;

    /* The known-good max-health hack works by cancelling this pre-death hurt
     * state. Catch it here so Phoenix prevents the vanilla transition instead
     * of trying to unwind game-over code after the fact. */
    if (state == PLAYER_STATE_HURT
        && (hp == 0 || hp == 0xFF || (flags & 0x1000) != 0))
        return 1;

    /* Stage handlers may leave HP at 0 without writing 0x2F — only treat
     * that as death when the hit flag is still set (not battle init). */
    if (hp == 0 && (flags & 0x1000) != 0)
        return 1;

    return 0;
}

/* Perform revive. No dying-state gate — callers decide when.
 * Do not arm gPhoenixReviveTailSkips: every known PlayerDeathFx caller
 * already jumps past DeleteActor via PhoenixDeathFxSkipReturn. A leftover
 * skip was softlocking overworld (save plug / rain / empty dialogue). */
APPEND_TEXT u32 DoPhoenixRevive(void)
{
    u8 *player = gPlayerPtr;

    NoCashGBAPrint("PHX DoRevive enter");
    if (!gRuntimeConfig.custom_gun_data)
    {
        NoCashGBAPrint("PHX reject: custom off");
        return 0;
    }
    if (player == NULL)
    {
        NoCashGBAPrint("PHX reject: no player");
        return 0;
    }
    if (!PhoenixIsEquipped())
    {
        NoCashGBAPrint("PHX reject: not equipped");
        return 0;
    }
    EnsurePhoenixReviveState(player);
    if (player[PLAYER_STATE_OFF] == PLAYER_STATE_SPAWN)
    {
        NoCashGBAPrint("PHX reject: spawn state");
        return 0;
    }
    if (gPhoenixReviveUsed)
    {
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
    *(u16 *)(player + PLAYER_FLAGS_OFF) &= (u16)~0x1000;
    /* Clear velocity so a death-zeroed ship doesn't softlock mid-explode. */
    *(u32 *)(player + 0x48) = 0;
    *(u32 *)(player + 0x54) = 0;
    return 1;
}

/* Called from hit-update / state-machine after death may have been written. */
APPEND_TEXT void ApplyPhoenixRevive(void)
{
    u8 *player = gPlayerPtr;

    if (!gRuntimeConfig.custom_gun_data || player == NULL)
        return;
    if (!PhoenixIsEquipped())
        return;
    EnsurePhoenixReviveState(player);

    if (player[PLAYER_STATE_OFF] == PLAYER_STATE_SPAWN)
        return;

    /* Vanilla often sets HP to 0xFF right before state 0x2F; stage paths
     * leave HP at 0 instead. Accept either signal. */
    if (PlayerNeedsPhoenixRevive(player))
    {
        NoCashGBAPrint("PHX Apply: needs revive");
        DoPhoenixRevive();
    }
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

/* Status UI ownership — unlock custom impacts before the ??? fallback. */
APPEND_TEXT u32 IsGunDataOwned__Replacement(u32 type, u32 local)
{
    EnsureCustomImpactsOwned();
    return IsGunDataOwned__Continue(type, local);
}

/* Redirect shooter gun-icon ANM to the extended blob with Suction frames. */
APPEND_TEXT void *GetArchiveFileStart__Replacement(u32 index)
{
    u32 ft = *CUR_ARCHIVE_FT_PTR;
    u32 *table;

    if (ft == SHOOTER_FILE_TABLE && index == GUN_ICON_FILE_INDEX && gGunIconAnmExtSize != 0)
        return gGunIconAnmExt;

    table = (u32 *)ft;
    return (void *)(ft + table[index]);
}

APPEND_TEXT u32 GetArchiveFileSize__Replacement(u32 index)
{
    u32 ft = *CUR_ARCHIVE_FT_PTR;
    u32 *entry;

    if (ft == SHOOTER_FILE_TABLE && index == GUN_ICON_FILE_INDEX && gGunIconAnmExtSize != 0)
        return gGunIconAnmExtSize;

    entry = (u32 *)(ft + index * 4);
    return entry[1] - entry[0];
}

/*
 * Custom impacts use extended ANM frames 196+ (one locked/owned pair each).
 * Vanilla pieces keep the Continue path.
 */
APPEND_TEXT void GetGunDataIconFrame__Replacement(u32 type, u32 local, u32 owned, u16 *out)
{
    u16 i;

    EnsureCustomImpactsOwned();

    if ((type & 0xFF) == 2)
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

    GetGunDataIconFrame__Continue(type, local, owned, out);
}

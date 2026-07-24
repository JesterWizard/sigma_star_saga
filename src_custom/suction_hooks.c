#include "global.h"
#include "runtime.h"
#include "ram_map.h"
#include "data_structures.h"
#include "suction.h"

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

APPEND_TEXT u32 EquippedImpactIndex(void)
{
    if (gGunLoadoutPrimaryFlag != 0)
        return gGunLoadoutImpact;
    return gGunLoadoutImpactAlt;
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

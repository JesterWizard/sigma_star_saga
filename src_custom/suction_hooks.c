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

/* Match vanilla gem magnet math (angle → sintab << 7), no range gate. */
APPEND_TEXT void AttractExpGemAtPlayerPtr(u32 pos_off)
{
    u8 *gem = gPlayerPtr;
    u8 *player = gActorPool;
    s32 *gx;
    s32 *gy;
    s32 px;
    s32 py;
    u32 angle;

    if (gem == NULL)
        return;

    gx = (s32 *)(gem + pos_off);
    gy = (s32 *)(gem + pos_off + 4);
    px = *(s32 *)(player + 0x40);
    py = *(s32 *)(player + 0x4C);
    angle = CALC_ANGLE(*gx, *gy, px, py) & 0xFF;
    *gx += ((s32)SIN_TABLE[angle + 64]) << 7;
    *gy += ((s32)SIN_TABLE[angle]) << 7;
}

APPEND_TEXT void ExpGemUpdate__Replacement(u32 arg)
{
    if (EquippedImpactIndex() == IMPACT_SUCTION)
        AttractExpGemAtPlayerPtr(0x18);
    ExpGemUpdate__Continue(arg);
}

APPEND_TEXT void LeechGemUpdate__Replacement(void)
{
    if (EquippedImpactIndex() == IMPACT_SUCTION)
        AttractExpGemAtPlayerPtr(0x40);
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
 * Custom impacts use ANM frames 196/197 (29_suction.png) from the extended
 * gun-icon archive. Vanilla pieces keep the Continue path.
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
                *out = (u16)(SUCTION_ICON_FRAME_LOCKED + ((owned & 0xFF) ? 1 : 0));
                return;
            }
        }
    }

    GetGunDataIconFrame__Continue(type, local, owned, out);
}

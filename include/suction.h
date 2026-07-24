#ifndef GUARD_SUCTION_H
#define GUARD_SUCTION_H

#include "gba/types.h"

/* Custom Impact Data (local index / Gun Data id). */
#define IMPACT_SUCTION 28
#define IMPACT_ID_SUCTION 77
#define IMPACT_SUCTION_PLUS 29
#define IMPACT_ID_SUCTION_PLUS 78
#define IMPACT_PHOENIX 30
#define IMPACT_ID_PHOENIX 79

/* Extended gun-icon ANM frames: custom impact i → 196+2*i (+1 if owned). */
#define SUCTION_ICON_FRAME_BASE 196

u32 EquippedImpactIndex(void);
u32 PhoenixIsEquipped(void);
/* 0 = not a suction impact; else sintab shift (7 = 1x, 8 = 2x). */
u32 EquippedSuctionSpeedShift(void);
void AttractExpGem(u8 *gem, u32 pos_off, u32 speed_shift);
void ApplyPhoenixRevive(void);
u32 DoPhoenixRevive(void);
void PlayerStateMachine__Replacement(void);
void PlayerHitUpdate__Replacement(void);
void PlayerDeathFx__Replacement(void);
void ExpGemUpdate__Replacement(u32 arg);
void LeechGemUpdate__Replacement(void);
void GetGunDataIconFrame__Replacement(u32 type, u32 local, u32 owned, u16 *out);
u32 IsGunDataOwned__Replacement(u32 type, u32 local);
void *GetArchiveFileStart__Replacement(u32 index);
u32 GetArchiveFileSize__Replacement(u32 index);

#endif /* GUARD_SUCTION_H */

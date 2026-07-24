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
#define IMPACT_NUM_PHOENIX 31 /* status-screen badge number */

/* Custom Cannon Data (local index / Gun Data id / badge number). */
#define CANNON_AUTO_TARGET 28
#define CANNON_ID_AUTO_TARGET 80
#define CANNON_NUM_AUTO_TARGET 29

/* Extended gun-icon ANM frames: custom impact i → 196+2*i (+1 if owned);
 * custom cannon i follows the impacts at 196+2*gCustomImpactCount+2*i. */
#define SUCTION_ICON_FRAME_BASE 196

u32 EquippedImpactIndex(void);
u32 PhoenixIsEquipped(void);
u32 EquippedCannonIndex(void);
u32 AutoTargetIsEquipped(void);
void ApplyAutoTarget(void);
/* 0 = not a suction impact; else sintab shift (7 = 1x, 8 = 2x). */
u32 EquippedSuctionSpeedShift(void);
void AttractExpGem(u8 *gem, u32 pos_off, u32 speed_shift);
void ApplyPhoenixRevive(void);
u32 DoPhoenixRevive(void);
u32 PhoenixDeathFxShouldSkip(void);
void SpawnPhoenixRevivePopup(void);
void TickPhoenixRevivePopup(void);
u32 PlayerLooksCrashed(u8 *player);
void LogPhoenixDeathFxEntry(void);
void LogPhoenixCrashProbe(const char *tag);
void PlayerStateMachine__Replacement(void);
void PlayerHitUpdate__Replacement(void);
void PlayerShipUpdate__Replacement(void);
void PlayerDeathFx__Replacement(void);
void ExpGemUpdate__Replacement(u32 arg);
void LeechGemUpdate__Replacement(void);
void GetGunDataIconFrame__Replacement(u32 type, u32 local, u32 owned, u16 *out);
u32 IsGunDataOwned__Replacement(u32 type, u32 local);
void *GetArchiveFileStart__Replacement(u32 index);
u32 GetArchiveFileSize__Replacement(u32 index);

#endif /* GUARD_SUCTION_H */

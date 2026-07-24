#ifndef GUARD_SUCTION_H
#define GUARD_SUCTION_H

#include "gba/types.h"

/* 29th Impact Data piece (local index 28; Gun Data ID 77). */
#define IMPACT_SUCTION 28
#define IMPACT_ID_SUCTION 77

/* Extended gun-icon ANM frames for Suction (locked/owned). */
#define SUCTION_ICON_FRAME_LOCKED 196
#define SUCTION_ICON_FRAME_OWNED 197

u32 EquippedImpactIndex(void);
void AttractExpGemAtPlayerPtr(u32 pos_off);
void ExpGemUpdate__Replacement(u32 arg);
void LeechGemUpdate__Replacement(void);
void GetGunDataIconFrame__Replacement(u32 type, u32 local, u32 owned, u16 *out);
u32 IsGunDataOwned__Replacement(u32 type, u32 local);
void *GetArchiveFileStart__Replacement(u32 index);
u32 GetArchiveFileSize__Replacement(u32 index);

extern u8 gGunIconAnmExt[];
extern u32 gGunIconAnmExtSize;

#endif /* GUARD_SUCTION_H */

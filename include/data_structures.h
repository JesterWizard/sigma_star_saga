#ifndef GUARD_DATA_STRUCTURES_H
#define GUARD_DATA_STRUCTURES_H

#include "gba/types.h"

typedef struct {
    u16 vanilla;
    u16 exp;
} EnemyExpRemapEntry;

typedef struct {
    u16 id;
    u16 exp;
} EnemyExpIdEntry;

/* One appended Impact Data piece (see impact_data.json).
 * Trailing pad keeps sizeof == 0x60 (agbcc aligns array elements). */
typedef struct {
    u8 index;     /* local impact index (28 = 29th piece) */
    u8 id;        /* Gun Data id */
    u8 number;    /* status-screen badge number */
    u8 icon_from; /* vanilla local index whose ANM frames to reuse */
    char desc[0x5A]; /* "NAME : text" status blurb */
    u8 pad[2];
} CustomImpactEntry;

#define CUSTOM_IMPACT_ENTRY_SIZE 0x60
#define VANILLA_IMPACT_COUNT 28
#define IMPACT_DESC_STRIDE 0x5A

extern const EnemyExpRemapEntry gEnemyExpRemap[];
extern const u16 gEnemyExpRemapCount;
extern const EnemyExpIdEntry gEnemyExpById[];
extern const u16 gEnemyExpByIdCount;

extern const CustomImpactEntry gCustomImpacts[];
extern const u16 gCustomImpactCount;

/* Sized from impact_data.json (vanilla 28 + custom count). */
extern u32 gImpactJumpTable[];
extern u8 gImpactDescTable[];
extern u8 gGunIconAnmExt[];
extern u32 gGunIconAnmExtSize;

/* Remap a vanilla flight-enemy gem amount using enemy_exp.json. */
u32 RemapEnemyExpAmount(u32 vanilla_amount);

/* Lookup by SpawnActor type id; returns vanilla_amount if missing. */
u32 LookupEnemyExpById(u16 enemy_id, u32 vanilla_amount);

/* If local is a custom impact, return icon_from; else local. */
u8 RemapCustomImpactIcon(u8 type, u8 local);

/* Mark JSON custom impacts owned (status screen does not run flight cheats). */
void EnsureCustomImpactsOwned(void);

#endif /* GUARD_DATA_STRUCTURES_H */

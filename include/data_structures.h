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

extern const EnemyExpRemapEntry gEnemyExpRemap[];
extern const u16 gEnemyExpRemapCount;
extern const EnemyExpIdEntry gEnemyExpById[];
extern const u16 gEnemyExpByIdCount;

/* Remap a vanilla flight-enemy gem amount using enemy_exp.json. */
u32 RemapEnemyExpAmount(u32 vanilla_amount);

/* Lookup by SpawnActor type id; returns vanilla_amount if missing. */
u32 LookupEnemyExpById(u16 enemy_id, u32 vanilla_amount);

#endif /* GUARD_DATA_STRUCTURES_H */

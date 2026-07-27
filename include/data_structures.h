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

/* Overworld fauna catalog entry (overworld_enemy_exp.json). */
typedef struct {
    u16 id;   /* SpawnActor type (actor+0x20) */
    u16 anim; /* ANM / model (actor+0x22) */
    u16 exp;
    u16 pad;
} OverworldEnemyExpEntry;

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

/* One appended Cannon Data piece (see cannon_data.json).
 * Trailing pad keeps sizeof == 0x58 (agbcc aligns array elements). */
typedef struct {
    u8 index;     /* local cannon index (28 = 29th piece) */
    u8 id;        /* Gun Data id */
    u8 number;    /* status-screen badge number */
    u8 icon_from; /* vanilla local index whose ANM frames to reuse */
    u8 fire_from; /* vanilla local index whose OnCannon handler to reuse */
    char desc[0x50]; /* "NAME : text" status blurb */
    u8 pad[3];
} CustomCannonEntry;

/* One appended Bullet Data piece (see bullet_data.json).
 * Trailing pad keeps sizeof == 0x4C (agbcc aligns array elements). */
typedef struct {
    u8 index;     /* local bullet index (20 = 21st piece) */
    u8 id;        /* Gun Data id */
    u8 number;    /* status-screen badge number */
    u8 icon_from; /* vanilla local index whose ANM frames to reuse */
    u8 shot_from; /* vanilla local index whose OnBullet handler to reuse */
    char desc[0x46]; /* "NAME : text" status blurb */
    u8 pad[1];
} CustomBulletEntry;

#define CUSTOM_IMPACT_ENTRY_SIZE 0x60
#define VANILLA_IMPACT_COUNT 28
#define IMPACT_DESC_STRIDE 0x5A

#define CUSTOM_CANNON_ENTRY_SIZE 0x58
#define VANILLA_CANNON_COUNT 28
#define CANNON_DESC_STRIDE 0x50

#define CUSTOM_BULLET_ENTRY_SIZE 0x4C
#define VANILLA_BULLET_COUNT 20
#define BULLET_DESC_STRIDE 0x46

extern const EnemyExpRemapEntry gEnemyExpRemap[];
extern const u16 gEnemyExpRemapCount;
extern const EnemyExpIdEntry gEnemyExpById[];
extern const u16 gEnemyExpByIdCount;

extern const OverworldEnemyExpEntry gOverworldEnemyExp[];
extern const u16 gOverworldEnemyExpCount;

extern const CustomImpactEntry gCustomImpacts[];
extern const u16 gCustomImpactCount;

extern const CustomCannonEntry gCustomCannons[];
extern const u16 gCustomCannonCount;

extern const CustomBulletEntry gCustomBullets[];
extern const u16 gCustomBulletCount;

/* Sized from impact_data.json (vanilla 28 + custom count). */
extern u32 gImpactJumpTable[];
extern u8 gImpactDescTable[];
/* Sized from cannon_data.json (vanilla 28 + custom count). */
extern u32 gCannonJumpTable[];
extern u8 gCannonDescTable[];
/* Sized from bullet_data.json (vanilla 20 + custom count). */
extern u32 gBulletJumpTable[];
extern u8 gBulletDescTable[];
extern u8 gGunIconAnmExt[];
extern u32 gGunIconAnmExtSize;

/* Remap a vanilla flight-enemy gem amount using enemy_exp.json. */
u32 RemapEnemyExpAmount(u32 vanilla_amount);

/* Lookup by SpawnActor type id; returns vanilla_amount if missing. */
u32 LookupEnemyExpById(u16 enemy_id, u32 vanilla_amount);

/* Overworld fauna: match type+anim; returns 0 if not in catalog. */
u32 LookupOverworldEnemyExp(u16 enemy_id, u16 anim);

/* If local is a custom impact, return icon_from; else local. */
u8 RemapCustomImpactIcon(u8 type, u8 local);

/* Mark JSON custom impacts owned (status screen does not run flight cheats). */
void EnsureCustomImpactsOwned(void);

/* Same for JSON custom cannons (gCannonOwned bit + Gun Data id bit). */
void EnsureCustomCannonsOwned(void);

/* Same for JSON custom bullets (gBulletOwned bit + Gun Data id bit). */
void EnsureCustomBulletsOwned(void);

#endif /* GUARD_DATA_STRUCTURES_H */

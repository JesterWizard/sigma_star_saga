#ifndef GUARD_RAM_MAP_H
#define GUARD_RAM_MAP_H

#include "gba/types.h"

/* Absolute addresses from asm/ram_map*.s (ygodm8-style). */

/* IWRAM — engine / player */
extern u8 gMode;
extern u8 *gPlayerPtr;
extern u8 gPlayerBombs;
extern u8 gPlayerLevel;
extern u32 gPlayerExp;
extern u8 gEnemyRemaining; /* Flight HUD quota / enemies left (not HP) */
extern u32 gExpToCurrentLevel;
extern u32 gExpToNextLevel;
extern u8 gStageClearFlag;
extern u8 gStageClearGate; /* 0 = allow clear (CB Clear Stage target) */

/* IWRAM — inventory */
extern u32 gGunDataBits[];
extern u32 gBulletOwned;
extern u32 gItemsOwned;
extern u32 gCannonOwned;
extern u32 gImpactOwned;

/* Last pool-backed vanilla IWRAM global (0x0300775C); free pool starts at 0x03007780. */
extern u8 gVanillaIwramHighWater;

/* Region free-space cursors (assembly .set symbols). */
extern u8 FreeRamSpaceTop;
extern u8 FreeRamSpaceBottom;
extern u8 FreeEwramSpaceTop;
extern u8 FreeEwramSpaceBottom;
extern u8 FreeFlashSpaceTop;
extern u8 FreeFlashSpaceBottom;

#endif /* GUARD_RAM_MAP_H */

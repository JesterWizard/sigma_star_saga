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
extern u8 gPlayerHp;
extern u32 gExpToCurrentLevel;
extern u32 gExpToNextLevel;
extern u8 gStageClearFlag;

/* IWRAM — inventory */
extern u32 gGunDataBits[];
extern u32 gBulletOwned;
extern u32 gItemsOwned;
extern u32 gCannonOwned;
extern u32 gImpactOwned;

/* Region free-space cursors (assembly .set symbols). */
extern u8 FreeRamSpaceTop;
extern u8 FreeRamSpaceBottom;
extern u8 FreeEwramSpaceTop;
extern u8 FreeEwramSpaceBottom;
extern u8 FreeFlashSpaceTop;
extern u8 FreeFlashSpaceBottom;

#endif /* GUARD_RAM_MAP_H */

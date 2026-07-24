#ifndef GUARD_RAM_MAP_H
#define GUARD_RAM_MAP_H

#include "gba/types.h"

/* Absolute addresses from asm/ram_map*.s (ygodm8-style). */

/* IWRAM — engine / player */
extern u8 gMode;
extern u8 *gPlayerPtr;
extern u8 *gExpGemPtr; /* Current gem in ExpGemUpdate (@ +0x18/+0x1C) */
extern u8 gActorPool[]; /* Flight actors, stride 0x60; [0] = ship */
extern u8 gPlayerBombs;
extern u8 gPlayerLevel;
extern u32 gPlayerExp;
extern u8 gEnemyRemaining; /* Flight HUD quota / enemies left (not HP) */
extern u32 gExpToCurrentLevel;
extern u32 gExpToNextLevel;
extern u8 gStageClearFlag;
extern u8 gStageClearGate; /* 0 = allow clear (CB Clear Stage target) */

/* IWRAM — inventory / gun loadout */
extern u32 gGunLoadout;
extern u32 gGunLoadoutImpact;
extern u32 gGunLoadoutImpactAlt;
extern u8 gGunLoadoutPrimaryFlag;
extern u32 gGunDataBits[];
extern u32 gBulletOwned;
extern u32 gItemsOwned;
extern u32 gCannonOwned;
extern u32 gImpactOwned;

/* Custom free-pool (via _kernel_malloc in ram_map_iwram.s). */
extern u8 gPhoenixReviveUsed;
extern u8 gPhoenixReviveMagicA;
extern u8 gPhoenixReviveMagicB;
extern u8 gPhoenixReviveTailSkips;
extern char gNoCashPrintBuf[];
extern u8 gInventoryCheatsApplied;
extern u8 gNoCashHeartbeat;
extern u8 gPhoenixCrashLogged;
extern u8 gPhoenixReviveAnmSwap;
extern u8 gPhoenixReviveAnmRestore;

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

#ifndef GUARD_RAM_MAP_H
#define GUARD_RAM_MAP_H

#include "gba/types.h"

/* Absolute addresses from asm/ram_map*.s (ygodm8-style). */

/* IWRAM — engine / player */
extern u8 gMode;
extern u8 *gPlayerPtr;
extern u16 gHeldKeys;
extern u16 gLastEncounterBattleId;
extern u8 *gMapObjBasePtr;
extern u8 gEncounterObjCount;
extern u8 gPlayerMapObjIndex;
extern u8 gEncounterObjIndices[];
extern u8 *gExpGemPtr; /* Current gem in ExpGemUpdate (@ +0x18/+0x1C) */
extern u8 gActorPool[]; /* Flight actors, stride 0x60; [0] = ship */
extern u16 gSoftOam[]; /* Soft OAM @ 0x03001F70 (8 bytes/entry) */
extern u8 gOamCursor;
extern u8 gSoftOamCount;
extern u8 gAffineSlotCounter;
extern u8 gActorOamStart;
extern u8 gUseYSorting;
extern u8 gCameras[]; /* Camera array, stride 0x84 */
extern u8 gPlayerBombs;
extern u8 gPlayerLevel;
extern u32 gPlayerExp;
extern u8 gEnemyRemaining; /* Flight HUD quota / enemies left (not HP) */
extern u32 gExpToCurrentLevel;
extern u32 gExpToNextLevel;
extern u8 gStageClearFlag;
extern u32 gRandomBattleCooldown; /* Overworld RNG encounter delay @ 0x1DA5C */
extern u8 gStageClearGate; /* 0 = allow clear (CB Clear Stage target) */

/* IWRAM — inventory / gun loadout */
extern u32 gGunLoadout;
extern u32 gGunLoadoutBullet;
extern u32 gGunLoadoutImpact;
extern u32 gGunLoadoutImpactAlt;
extern u8 gGunLoadoutPrimaryFlag;
extern u32 gGunDataBits[];
extern u32 gBulletOwned;
extern u32 gItemsOwned;
extern u32 gCannonOwned;
extern u32 gImpactOwned;
extern u8 gShotAuxPool[]; /* Flight shot aux, stride 0x40 (vanilla 10 slots) */
extern u8 gShotSlotCounter; /* AllocShotSlot ring @ 0x030002F0 */

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
extern u32 gActorMaxHp[];
extern u8 gHpBarTilesReady;
extern u32 gHpBarTileScratch[];
extern u8 gChargeShotPhase;
extern u16 gChargeShotTimer;

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

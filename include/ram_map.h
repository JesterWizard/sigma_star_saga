#ifndef GUARD_RAM_MAP_H
#define GUARD_RAM_MAP_H

#include "gba/types.h"

/* Absolute addresses from asm/ram_map*.s (ygodm8-style). */

/* IWRAM — GAX */
#include "gax.h"
extern Gax2Params gGaxParams; /* @ 0x030008C0 */
extern void *gGaxCurrentSong; /* @ 0x030008B0 */
extern u8 gGaxMixBuffer[]; /* @ 0x03005910, 0x1000 bytes (IWRAM) */
extern u8 gGaxSpeechObject[]; /* @ 0x03004348, 0x7A4 bytes (IWRAM) */
extern void *gGaxWorkspacePtr; /* @ 0x0300775C — pointer slot, not the buffer */
extern u16 gGaxMusicVol; /* @ 0x030011FC */

/* IWRAM — engine / player */
extern u8 gMode;
extern u8 gCurrentActorIndex;
extern u8 *gPlayerPtr;
extern u16 gHeldKeys;
extern u16 gLastEncounterBattleId;
extern u8 *gMapObjBasePtr;
extern u8 gEncounterObjCount;
extern u8 gPlayerMapObjIndex;
extern u8 gEncounterObjIndices[];
extern u8 *gExpGemPtr; /* Current gem in ExpGemUpdate (@ +0x18/+0x1C) */
extern u8 gActorPool[]; /* Actors, stride 0x60; flight [0]=ship; cutscenes reuse */
extern u8 gActorAllocIndex; /* SpawnActor free-slot scan start @ 0x0300368C */
extern u32 gCutsceneCounter; /* Cutscene wait counter / spawn gate @ 0x03003688 */
extern u16 gSoftOam[]; /* Soft OAM @ 0x03001F70 (8 bytes/entry) */
extern u8 gOamCursor;
extern u8 gSoftOamCount;
extern u8 gAffineSlotCounter;
extern u8 gActorOamStart;
extern u8 gUseYSorting;
extern u8 gCameras[]; /* Camera array, stride 0x84 */
extern const u8 *const *gDialogueBankBases; /* IWRAM ptr to 7 bank bases @ 0x03000018 */
extern u32 gCutsceneStep; /* Per-frame cutscene step @ 0x03007704 */
extern u32 gCutsceneParam[]; /* Lerp pair [0]/[1] @ 0x03007708 */
extern u32 gStageCase; /* Stage FSM outer JT index @ 0x03007738 */
extern u16 gCutsceneGateHw; /* Stage intro wait halfword @ 0x030038A8 */
extern u32 gWorldScrollX; /* World/cutscene scroll X (16.16) @ 0x03000C44 */
extern u32 gWorldScrollY; /* World/cutscene scroll Y (16.16) @ 0x03000C6C */
extern u16 gCutsceneWaveOutput[]; /* Case 62 waveform output @ 0x030036A0 */
extern u8 gCutsceneWavePhase; /* Case 62 waveform phase @ 0x030076CC */
extern u16 gDialogueEntryOffsets[]; /* Per-script halfword offsets @ 0x02000010 */
extern u8 gTalkState[]; /* Talk UI block @ 0x03007100; flags halfword at +14 */
extern const u8 *gTalkStreamPtr; /* Active talk stream cursor @ 0x03007198 */
extern u8 gTalkParamA;
extern u8 gTalkParamB;
extern u8 gTalkBusyA;
extern u8 gTalkBusyB;
extern u8 gTalkParamAMirror;
extern u8 gTalkLayout;
extern u8 gTalkPagePos;
extern u8 gTalkPagePos2;
extern u8 gTalkLayoutCmp;
extern u8 gTalkActive;
extern u8 gTalkGate;
extern u8 gTalkExtra; /* StartTalkByIdEx latch / TalkAdvance speaker @ 0x0300001C */
extern u8 gTalkPortraitSide; /* TalkAdvance side @ 0x0300001D */
extern u8 gTalkPortraitExpr; /* TalkAdvance expr @ 0x0300001E */
extern u8 gPlayerBombs;
extern u8 gPlayerLevel;
extern u32 gPlayerExp;
extern u32 gPlayerExpDisplay; /* Status/HUD mirror; HudSync copies from gPlayerExp */
extern u8 gEnemyRemaining; /* Flight HUD quota / enemies left (not HP) */
extern u32 gExpToCurrentLevel;
extern u32 gExpToNextLevel;
extern u8 gStageClearFlag;
extern u32 gRandomBattleCooldown; /* Overworld RNG encounter delay @ 0x1DA5C */
extern u8 gEventFlags[]; /* Story/tool bitfield @ 0x03000020 (GetFlag/SetFlag) */
extern u8 gEquippedTool; /* Active overworld tool index 0..5 @ 0x03007064 */
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

/* Last pool-backed vanilla IWRAM global before free pool (see gGaxWorkspacePtr). */
extern u8 gVanillaIwramHighWater;

/* Region free-space cursors (assembly .set symbols). */
extern u8 FreeRamSpaceTop;
extern u8 FreeRamSpaceBottom;
extern u8 FreeEwramSpaceTop;
extern u8 FreeEwramSpaceBottom;
extern u8 FreeFlashSpaceTop;
extern u8 FreeFlashSpaceBottom;

#endif /* GUARD_RAM_MAP_H */

#ifndef GUARD_DIALOGUE_H
#define GUARD_DIALOGUE_H

#include "global.h"

/* ROM talk-bank pointer table (7 entries) @ file 0x24EA6C. */
#define gDialogueBankTable ((const u8 *const *)0x0824EA6C)

/* Script-ID ranges: 7× (lo, hi) u32 @ file 0x5BF3C. */
#define gDialogueIdRanges ((const u32 *)0x0805BF3C)

#define TALK_STATE_FLAGS_OFF 14
#define TALK_FLAG_BUSY       0x0008

/* Vanilla @ 0x08010808 — arm talk UI from a stream pointer. */
void StartTalkPtr(const u8 *stream, u8 paramA, u8 paramB);

/* Vanilla @ 0x080108B0 — resolve script ID → stream, then StartTalkPtr. */
void StartTalkById(u16 scriptId, u8 paramA, u8 paramB);

/* Vanilla @ 0x0801093C — StartTalkById + store extra byte @ gTalkGate-adjacent. */
void StartTalkByIdEx(u16 scriptId, u8 paramA, u8 paramB, u8 extra);

/* Vanilla @ 0x08010964 — scan bank for '#' → gDialogueEntryOffsets[id]. */
void BuildTalkOffsets(const u8 *stream, s32 startId, s32 endId);

/* Vanilla @ 0x080109A0 — install bank table ptr + build all offsets. */
void InitTalkBanks(void);

#endif /* GUARD_DIALOGUE_H */

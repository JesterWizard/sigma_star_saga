#ifndef GUARD_DIALOGUE_H
#define GUARD_DIALOGUE_H

#include "global.h"

/* ROM talk-bank pointer table (7 entries) @ file 0x24EA6C. */
#define gDialogueBankTable ((const u8 *const *)0x0824EA6C)

/* Script-ID ranges: 7× (lo, hi) u32 @ file 0x5BF3C. */
#define gDialogueIdRanges ((const u32 *)0x0805BF3C)

#define TALK_STATE_FLAGS_OFF 14
#define TALK_FLAG_BUSY       0x0008

/* Voice cue sentinel — stop speech without playing a new clip. */
#define TALK_VOICE_STOP 0xFFFF

typedef struct {
    const u8 *header; /* points at leading 0x07 of a TALK header */
    u16 voiceId;      /* catalog id, or TALK_VOICE_STOP */
} TalkVoiceCue;

extern const TalkVoiceCue gTalkVoiceCues[];
extern const u16 gTalkVoiceCueCount;

/* Vanilla @ 0x080102BC — consume `\x07 speaker side expr \x07`, return text ptr. */
const u8 *TalkAdvance(const u8 *stream);

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

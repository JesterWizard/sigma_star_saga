#ifndef GUARD_GAX_H
#define GUARD_GAX_H

#include "gba/types.h"

/* GAX Sound Engine 3.05A — thin C surface over vanilla stubs / workspace. */

#define GAX_FLAG_SPEECH 0x0040

#define GAX_SPEECH_ID_BASE 0x100
#define GAX_SPEECH_FRAME_BYTES 0x21
#define GAX_SPEECH_OBJECT_BYTES 0x7A4
/* Custom tail beyond the vanilla object: temporary consumer-probe latch.
 * Lives inside our own allocation (NOT the free pool — that region is user
 * stack headroom and gets thrashed at runtime). */
#define GAX_SPEECH_OBJECT_TOTAL_BYTES 0x7A8
#define gGaxSpeechProbeCount \
    (*(volatile u8 *)(gGaxSpeechObject + GAX_SPEECH_OBJECT_BYTES))
/* FX voice channel bookkeeping, in the same custom tail (+0x7A6). */
#define gGaxVoiceFxChannel \
    (*(volatile s16 *)(gGaxSpeechObject + GAX_SPEECH_OBJECT_BYTES + 2))

#define GAX_MIX_BUFFER_BYTES 0x1000
#define GAX_PACKAGE_ROM ((const void *)0x0824BE44)

typedef struct {
    void *wram;              /* +0x00 mix / workspace */
    u32 wramSize;            /* +0x04 */
    s16 mixingRate;          /* +0x08 */
    s16 fxMixingRate;        /* +0x0A */
    s16 field0C;             /* +0x0C */
    u16 flags;               /* +0x0E — bit 0x40 = GAX_SPEECH */
    s16 numFxChannels;       /* +0x10 */
    s16 volume;              /* +0x12 */
    u8 unknown14[0x1C];      /* +0x14 .. +0x2F */
    const void *package;     /* +0x30 music&FX package */
    const void *music;       /* +0x34 song module */
    u32 field38;             /* +0x38 */
    u8 debugAssert;          /* +0x3C */
    u8 field3D;
    u8 field3E;
    u8 field3F;
} Gax2Params;

/* Speech table entry: frames at [0, bitOffset), bitstream at bitOffset+. */
typedef struct {
    const u8 *data;
    u32 sizeFlags; /* bit31 set; low 29 = byte offset of bitstream */
} GaxSpeechEntry;

/* Fake package-like owner: consumer reads *(speechObj+0)+0x14 as the table. */
typedef struct {
    u32 pad[5];
    const GaxSpeechEntry *table; /* +0x14 */
} GaxSpeechOwnerHeader;

/* ROM-resident owner: the IWRAM free pool overlaps user-stack headroom, so a
 * RAM owner gets stomped by deep stacks (title/talk) and the IRQ-time
 * consumer dereferences a wild table pointer.  ROM is immune; nothing in the
 * GAX speech path writes through the owner. */
extern const GaxSpeechOwnerHeader gGaxSpeechOwnerRom;

void PlaySfx(u32 id, u32 pan);
/* FX channel-playing variants (voice path). Returns FX channel index. */
s32 PlaySfxEx(u32 id, s32 channel, u32 priority, u32 note);
void StopFxChannel(s32 channel);
void PlayBgm(const void *songModule);
void StopBgm(void);
void StopAllFx(void);
void SetMusicVol(u32 channelOrMinus1, u32 vol0to255);
void SetMasterVol(u32 vol0toFFFF);

void Gax2New(Gax2Params *params);
void Gax2Init(Gax2Params *params);
/* Attach IWRAM speech object + catalog; sets GAX_FLAG_SPEECH after init. */
void GaxAttachSpeech(void);

/* Vanilla boot fragment: gax2_new → wire params → gax2_init (@ 0x080038D8). */
void GaxBootInit(void);

#endif /* GUARD_GAX_H */

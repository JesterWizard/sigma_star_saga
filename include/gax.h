#ifndef GUARD_GAX_H
#define GUARD_GAX_H

#include "gba/types.h"

/* GAX Sound Engine 3.05A — thin C surface over vanilla stubs / workspace. */

#define GAX_FLAG_SPEECH 0x0040

#define GAX_SPEECH_ID_BASE 0x100
#define GAX_SPEECH_FRAME_BYTES 0x21

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

void PlaySfx(u32 id, u32 pan);
void PlayBgm(const void *songModule);
void StopBgm(void);
void StopAllFx(void);
void SetMusicVol(u32 channelOrMinus1, u32 vol0to255);
void SetMasterVol(u32 vol0toFFFF);

void Gax2New(Gax2Params *params);
void Gax2Init(Gax2Params *params);
/* No-op until speech frames are IRQ-safe; kept for call-site stability. */
void GaxAttachSpeech(void);

/* Vanilla boot fragment: gax2_new → wire params → gax2_init (@ 0x080038D8). */
void GaxBootInit(void);

#endif /* GUARD_GAX_H */

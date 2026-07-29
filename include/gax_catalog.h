#ifndef GUARD_GAX_CATALOG_H
#define GUARD_GAX_CATALOG_H

#include "gba/types.h"
#include "gax.h"

#define GAX_MUSIC_COUNT 1
#define GAX_VOICE_COUNT 2

#define GAX_MUSIC_TEST_BGM 0
#define GAX_VOICE_TIERNEY_BRIEFING 1

extern const u16 gGaxMusicCount;
extern const u16 gGaxVoiceCount;
extern const void *const gGaxMusicTable[];
extern const GaxSpeechEntry gGaxVoiceTable[];

/* FX voice: gax_fx id for clip N = GAX_VOICE_FX_BASE + N. */
#define GAX_VOICE_FX_BASE 124
#define GAX_VOICE_FX_NOTE 58
#define GAX_VOICE_FX_PRIORITY 0x7FFF
#define GAX_FX_WAVESET_COUNT 175

typedef struct { const u8 *addr; u32 size; } GaxFxWaveEntry;
typedef struct {
    const u8 *data;
    u32 bytes;
    u16 waveSlot;
} GaxVoiceDpcmClip;

extern const u32 gGaxPackageEx[];
extern const GaxFxWaveEntry gGaxFxWaveSetEx[];
extern const GaxVoiceDpcmClip gGaxVoiceDpcmTable[];

#endif /* GUARD_GAX_CATALOG_H */

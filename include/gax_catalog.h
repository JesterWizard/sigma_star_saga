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
extern const u32 gGaxPackageEx[];

#endif /* GUARD_GAX_CATALOG_H */

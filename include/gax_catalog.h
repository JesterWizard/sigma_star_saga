#ifndef GUARD_GAX_CATALOG_H
#define GUARD_GAX_CATALOG_H

#include "gba/types.h"
#include "gax.h"

#define GAX_MUSIC_COUNT 1
#define GAX_VOICE_COUNT 2

#define GAX_MUSIC_TEST_BGM 0
#define GAX_VOICE_TEST_LINE 0
#define GAX_VOICE_TIERNEY_BRIEFING 1

extern const u16 gGaxMusicCount;
extern const u16 gGaxVoiceCount;
extern const void *const gGaxMusicTable[];
extern const GaxSpeechEntry gGaxVoiceTable[];

#endif /* GUARD_GAX_CATALOG_H */

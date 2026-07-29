#ifndef GUARD_GAX_VOICE_DPCM_H
#define GUARD_GAX_VOICE_DPCM_H

#include "gba/types.h"

/* Decode VD1 4-bit delta DPCM into unsigned PCM. Returns sample count, or 0. */
u32 GaxVoiceDecodeDpcm(const u8 *src, u32 srcBytes, u8 *dst, u32 dstCap);

#endif /* GUARD_GAX_VOICE_DPCM_H */

#include "global.h"
#include "ram_map.h"
#include "runtime.h"

/*
 * Signed 4-bit delta DPCM (VD1 from pack_gax_voice_fx.py).
 * Header: magic, sampleCount, firstSample, step, pad[2].
 * Each nibble q in [-8,7] advances the predictor by q*step (step 0 → 1).
 * Decode once on the main thread into gVoiceDecodeBuf — never in the mix IRQ.
 */

APPEND_TEXT u32 GaxVoiceDecodeDpcm(const u8 *src, u32 srcBytes, u8 *dst, u32 dstCap)
{
    u32 cap = dstCap;
    u32 sampleCount;
    u32 pred;
    u32 i;
    u32 need;
    u32 bodyBytes;
    u32 step;
    const u8 *body;
    u8 first;

    if (src == NULL || dst == NULL || srcBytes < 12)
        return 0;
    if (src[0] != 'V' || src[1] != 'D' || src[2] != '1' || src[3] != 0)
        return 0;

    sampleCount = src[4] | ((u32)src[5] << 8) | ((u32)src[6] << 16)
        | ((u32)src[7] << 24);
    first = src[8];
    step = src[9];
    if (step == 0)
        step = 1;
    if (sampleCount == 0 || sampleCount > cap)
        return 0;

    dst[0] = first;
    pred = first;
    body = src + 12;
    bodyBytes = srcBytes - 12;
    need = sampleCount - 1;

    for (i = 0; i < need; i++)
    {
        u32 bi = i >> 1;
        s32 q;
        s32 next;
        u8 n;

        if (bi >= bodyBytes)
            return 0;
        if ((i & 1) == 0)
            n = (u8)((body[bi] >> 4) & 0xF);
        else
            n = (u8)(body[bi] & 0xF);
        if (n >= 8)
            q = (s32)n - 16;
        else
            q = (s32)n;
        next = (s32)pred + q * (s32)step;
        if (next < 0)
            next = 0;
        else if (next > 255)
            next = 255;
        pred = (u32)next;
        dst[i + 1] = (u8)pred;
    }

    return sampleCount;
}

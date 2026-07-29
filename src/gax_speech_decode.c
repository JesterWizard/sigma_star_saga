#include "global.h"
#include "gax_speech.h"

/*
 * Decompiled field unpack for GAX speech frame decode @ 0x0805708C.
 * Runtime playback still uses the vanilla ROM body (no LynJump on decode).
 * This peel documents the packer contract: 0xD nibble + 260 MSB-first bits
 * into 76 halfwords at speechState+0x3F8..+0x48E.
 *
 * Vanilla signature (Thumb):
 *   s32 GaxSpeechDecode(u8 *speechStatePlus4, const u8 *frame, s16 *pcmOut);
 * Consumer passes speech_object+4, frame pointer, speech_object+0x494.
 *
 * Synthesis chain (ROM, stateful across frames):
 *   0x08056F88 pitch/energy setup, 0x08056B74 coeff expand, 0x08056D34 mix,
 *   indirect stub via object+0x794 installed by 0x0805773C.
 */

/* Ordered MSB-first after the 0xD header nibble. Widths sum to 260. */
static const u16 sGaxSpeechFieldOff[GAX_SPEECH_FIELD_COUNT] = {
    0x3F8, 0x3FA, 0x3FC, 0x3FE, 0x400, 0x402, 0x404, 0x406, 0x408, 0x418,
    0x410, 0x420, 0x428, 0x42A, 0x42C, 0x42E, 0x430, 0x432, 0x434, 0x436,
    0x438, 0x43A, 0x43C, 0x43E, 0x440, 0x40A, 0x41A, 0x412, 0x422, 0x442,
    0x444, 0x446, 0x448, 0x44A, 0x44C, 0x44E, 0x450, 0x452, 0x454, 0x456,
    0x458, 0x45A, 0x40C, 0x41C, 0x414, 0x424, 0x45C, 0x45E, 0x460, 0x462,
    0x464, 0x466, 0x468, 0x46A, 0x46C, 0x46E, 0x470, 0x472, 0x474, 0x40E,
    0x41E, 0x416, 0x426, 0x476, 0x478, 0x47A, 0x47C, 0x47E, 0x480, 0x482,
    0x484, 0x486, 0x488, 0x48A, 0x48C, 0x48E,
};

static const u8 sGaxSpeechFieldWidth[GAX_SPEECH_FIELD_COUNT] = {
    6, 6, 5, 5, 4, 4, 3, 3, 7, 2, 2, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    7, 2, 2, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 2, 2, 6, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 2, 2, 6, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3,
};

s32 GaxSpeechFrameValidate(const u8 *frame)
{
    if (frame == NULL)
        return -1;
    if ((frame[0] >> 4) != GAX_SPEECH_FRAME_NIBBLE)
        return -1;
    return 0;
}

s32 GaxSpeechDecodeUnpack(u8 *speechStatePlus4, const u8 *frame)
{
    u32 bitPos;
    u32 i;
    u32 width;
    u32 value;
    u32 b;
    u32 srcBit;
    u32 bit;

    if (speechStatePlus4 == NULL)
        return -1;
    if (GaxSpeechFrameValidate(frame) != 0)
        return -1;

    /* Payload starts after the 4-bit 0xD header in byte0. */
    bitPos = 4;
    for (i = 0; i < GAX_SPEECH_FIELD_COUNT; i++)
    {
        width = sGaxSpeechFieldWidth[i];
        value = 0;
        for (b = 0; b < width; b++)
        {
            srcBit = 7 - (bitPos & 7);
            bit = (frame[bitPos >> 3] >> srcBit) & 1;
            value = (value << 1) | bit;
            bitPos++;
        }
        *(u16 *)(speechStatePlus4 + sGaxSpeechFieldOff[i]) = (u16)value;
    }

    return (bitPos == 4 + GAX_SPEECH_PAYLOAD_BITS) ? 0 : -1;
}

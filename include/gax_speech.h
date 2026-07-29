#ifndef GUARD_GAX_SPEECH_H
#define GUARD_GAX_SPEECH_H

#include "gba/types.h"
#include "gax.h"

/*
 * GAX_SPEECH frame decode / consumer contract (vanilla @ 0x0805708C / 0x08056A30).
 *
 * Frame layout (GAX_SPEECH_FRAME_BYTES = 0x21):
 *   byte0 high nibble must be 0xD (else decode returns -1 immediately)
 *   remaining 260 bits unpack MSB-first into 76 halfwords at state+0x3F8..+0x48E
 *   synthesis then runs ROM helpers (0x08056F88, 0x080589C8, 0x08056D34, 0x08056B74)
 *
 * Blob layout for a catalog entry:
 *   [0, bit_offset): packed frames (multiple of 0x21)
 *   [bit_offset, …): 1-bit gate stream (MSB-first within each byte)
 *   sizeFlags = 0x80000000 | bit_offset
 *
 * Speech object (size GAX_SPEECH_OBJECT_BYTES):
 *   +0x000  owner header pointer — *(owner+0x14) = GaxSpeechEntry table
 *   +0x494  decode PCM scratch (consumer passes obj+0x494 as out)
 *   +0x798  active voice index; idle = -1
 *   +0x79C  frame byte cursor into entry->data
 *   +0x7A0  gate bit index
 *
 * Workspace (gGaxWorkspacePtr):
 *   +0x08  package pointer (vanilla post-init may store into speech+0)
 *   +0x14  speech object pointer (consumer / PlaySfx speech path)
 *   +0x24  rate-block pointer (source)
 *   +0x2C  active rate-block pointer (consumer reads *(+0x2C)+4 before gate)
 *   +0x38  mix destination used after decode
 *
 * Consumer @ 0x08056A30 (called from mixer when GAX_FLAG_SPEECH set):
 *   if index == -1 → return
 *   entry = table[index]; require sizeFlags bit31
 *   bit_offset = sizeFlags & 0x1FFFFFFF
 *   if frame_cursor >= bit_offset → store -1 to index (auto-stop)
 *   else read gate bit at data[bit_offset + bit_index/8]
 *   if gate==1 → GaxSpeechDecode(obj+4, data+frame_cursor, obj+0x494),
 *                mix via 0x080589C8, frame_cursor += 0x21, bit_index++
 *   if gate==0 → bit_index++ only
 *
 * Init helper @ 0x0805770C: codec reset on obj+4, index=-1, cursors=0;
 * copies Thumb mix stubs into the object and installs +0x78C/+0x790/+0x794.
 */

#define GAX_SPEECH_FRAME_NIBBLE 0xD
#define GAX_SPEECH_PAYLOAD_BITS 260
#define GAX_SPEECH_FIELD_COUNT 76
#define GAX_SPEECH_SAMPLES_PER_FRAME 40

#define GAX_SPEECH_INDEX_OFF 0x798
#define GAX_SPEECH_FRAME_CURSOR_OFF 0x79C
#define GAX_SPEECH_BIT_INDEX_OFF 0x7A0
#define GAX_SPEECH_PCM_OFF 0x494
#define GAX_SPEECH_OWNER_TABLE_OFF 0x14
#define GAX_SPEECH_MIX_FN_OFF 0x794

#define GAX_WS_PACKAGE_OFF 0x08
#define GAX_WS_SPEECH_OBJ_OFF 0x14
#define GAX_WS_RATE_SRC_OFF 0x24
#define GAX_WS_RATE_OFF 0x2C
#define GAX_WS_MIX_DEST_OFF 0x38

#define GAX_SPEECH_SIZEFLAGS_BIT 0x80000000u
#define GAX_SPEECH_BIT_OFFSET_MASK 0x1FFFFFFFu

/* Early reject / accept for a single frame header (matches 0x0805708C prologue). */
s32 GaxSpeechFrameValidate(const u8 *frame);

/*
 * Full 260-bit unpack into halfwords at vanilla speech-state offsets
 * (relative to speech object + 4). Does not run synthesis — ROM @ 0x0805708C
 * remains authoritative at runtime.
 */
s32 GaxSpeechDecodeUnpack(u8 *speechStatePlus4, const u8 *frame);

#endif /* GUARD_GAX_SPEECH_H */

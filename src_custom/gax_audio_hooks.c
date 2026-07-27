#include "global.h"
#include "dialogue.h"
#include "gax.h"
#include "gax_audio.h"
#include "gax_catalog.h"
#include "ram_map.h"
#include "runtime.h"

/*
 * Custom GAX music / voice hooks.
 *
 * IMPORTANT: Do not set GAX_FLAG_SPEECH during boot or BGM re-init. With our
 * unfinished speech bitstream, the GAX audio IRQ faults as soon as IE is
 * enabled → white screen. Mix buffer stays vanilla IWRAM 0x03005910 / 0x1000.
 *
 * Voice cues are accepted by the dialogue compiler; playback stays gated until
 * the speech encoder matches 0x0805708C.
 */

LYN_REPLACE_CHECK(GaxBootInit__Replacement);

extern void GaxBootInit__Continue(void);

APPEND_TEXT void GaxAttachSpeech(void)
{
    /* Intentionally empty until speech frames are IRQ-safe. */
}

APPEND_TEXT void GaxBootInit__Replacement(void)
{
    Gax2New(&gGaxParams);
    gGaxParams.debugAssert = 0;
    gGaxParams.wramSize = GAX_MIX_BUFFER_BYTES;
    gGaxParams.wram = gGaxMixBuffer;
    gGaxParams.package = GAX_PACKAGE_ROM;
    /* Keep speech disabled — IRQ-safe. */
    gGaxParams.flags = (u16)(gGaxParams.flags & ~GAX_FLAG_SPEECH);

    Gax2Init(&gGaxParams);
    GaxBootInit__Continue();
}

APPEND_TEXT void GaxPlayMusic(u16 id)
{
    if (!gRuntimeConfig.custom_gax_audio)
        return;
    if (id >= gGaxMusicCount || gGaxMusicTable[id] == NULL)
        return;
    PlayBgm(gGaxMusicTable[id]);
}

APPEND_TEXT void GaxStopMusic(void)
{
    StopBgm();
}

APPEND_TEXT void GaxPlayVoice(u16 id, s16 pan)
{
    (void)id;
    (void)pan;
    /* Gated: PlaySfx(0x100+id) + GAX_SPEECH currently white-screens via IRQ. */
}

APPEND_TEXT void GaxStopVoice(void)
{
}

static void ApplyTalkVoiceCue(const u8 *header)
{
    u16 i;

    if (!gRuntimeConfig.custom_gax_audio)
        return;

    for (i = 0; i < gTalkVoiceCueCount; i++)
    {
        if (gTalkVoiceCues[i].header != header)
            continue;
        if (gTalkVoiceCues[i].voiceId == TALK_VOICE_STOP)
            GaxStopVoice();
        else
            GaxPlayVoice(gTalkVoiceCues[i].voiceId, 0);
        return;
    }
}

LYN_REPLACE_CHECK(TalkAdvance_Gax__Replacement);

APPEND_TEXT const u8 *TalkAdvance_Gax__Replacement(const u8 *stream)
{
    ApplyTalkVoiceCue(stream);
    return TalkAdvance(stream);
}

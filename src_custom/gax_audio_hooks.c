#include "global.h"
#include "dialogue.h"
#include "gax.h"
#include "gax_audio.h"
#include "gax_catalog.h"
#include "gax_speech.h"
#include "nocash.h"
#include "ram_map.h"
#include "runtime.h"

/*
 * Custom GAX music / voice hooks.
 *
 * Mix stays vanilla IWRAM 0x03005910 / 0x1000. Speech object is separate at
 * gGaxSpeechObject (0x03004348 / 0x7A4). Owner header is ram-map IWRAM (never
 * .bss — the linker places .bss at 0x03000000 and stomps vanilla RAM).
 *
 * Vanilla speech contract (Gax2Init carve @ 0x080551AC + post-init @ 0x08054EDC):
 *   workspace+0x14 = speech object
 *   workspace+0x2C = workspace+0x24  (rate block; consumer reads *(+0x2C)+4)
 *   speech object init @ 0x0805770C
 * Standalone attach mirrors those fields without carving 0x7A4 out of the mix.
 */

LYN_REPLACE_CHECK(GaxBootInit__Replacement);
LYN_REPLACE_CHECK(GaxSpeechConsumer__Replacement);

extern void GaxBootInit__Continue(void);
extern s32 GaxSpeechConsumer__Continue(void *speechObj);

/* Vanilla @ 0x0805770C — reset codec state, index=-1, cursors=0. */
typedef void (*GaxSpeechObjectInitFunc)(void *speechObj);
#define GaxSpeechObjectInitRom ((GaxSpeechObjectInitFunc)0x0805770D)

static void GaxLogSpeechContract(const char *tag, void *workspace)
{
    u8 *ws = (u8 *)workspace;
    void *rate;
    s32 index;
    u32 frameCursor;
    u32 bitIndex;
    u32 mixFn;

    if (ws == NULL)
    {
        NoCashGBAPrintf("GAX %s ws=NULL", tag);
        return;
    }

    rate = *(void **)(ws + GAX_WS_RATE_OFF);
    index = *(s32 *)(gGaxSpeechObject + GAX_SPEECH_INDEX_OFF);
    frameCursor = *(u32 *)(gGaxSpeechObject + GAX_SPEECH_FRAME_CURSOR_OFF);
    bitIndex = *(u32 *)(gGaxSpeechObject + GAX_SPEECH_BIT_INDEX_OFF);
    mixFn = *(u32 *)(gGaxSpeechObject + GAX_SPEECH_MIX_FN_OFF);

    NoCashGBAPrintf(
        "GAX %s ws=%x +14=%x +2C=%x obj=%x idx=%d fc=%u bi=%u mix=%x fl=%x",
        tag,
        (u32)ws,
        *(u32 *)(ws + GAX_WS_SPEECH_OBJ_OFF),
        (u32)rate,
        (u32)gGaxSpeechObject,
        index,
        frameCursor,
        bitIndex,
        mixFn,
        (u32)gGaxParams.flags);
}

APPEND_RODATA const GaxSpeechOwnerHeader gGaxSpeechOwnerRom = {
    .pad = {0, 0, 0, 0, 0},
    .table = gGaxVoiceTable,
};

/* Attached iff the workspace still points at our object with the ROM owner. */
static s32 GaxSpeechIsAttached(void)
{
    u8 *ws = (u8 *)gGaxWorkspacePtr;
    if (ws == NULL)
        return 0;
    if (*(void **)(ws + GAX_WS_SPEECH_OBJ_OFF) != (void *)gGaxSpeechObject)
        return 0;
    return *(void **)gGaxSpeechObject == (void *)&gGaxSpeechOwnerRom;
}

APPEND_TEXT void GaxAttachSpeech(void)
{
    void *workspace;
    u8 *ws;
    u32 i;
    u16 ime;

    if (!gRuntimeConfig.custom_gax_audio)
        return;

    /*
     * The GAX mixer reads this object from IRQ context.  Disable master IRQ
     * before clearing/replacing it; otherwise an active old voice can observe
     * a half-written owner pointer or codec stub.
     */
    ime = REG_IME;
    REG_IME = 0;
    gGaxParams.flags = (u16)(gGaxParams.flags & ~GAX_FLAG_SPEECH);

    workspace = gGaxWorkspacePtr;
    if (workspace == NULL)
    {
        REG_IME = ime;
        return;
    }

    ws = (u8 *)workspace;

    for (i = 0; i < GAX_SPEECH_OBJECT_BYTES; i++)
        gGaxSpeechObject[i] = 0;

    *(const GaxSpeechOwnerHeader **)gGaxSpeechObject = &gGaxSpeechOwnerRom;

    GaxSpeechObjectInitRom(gGaxSpeechObject);

    /* Re-install custom owner after ROM init (vanilla would store package@+0). */
    *(const GaxSpeechOwnerHeader **)gGaxSpeechObject = &gGaxSpeechOwnerRom;
    *(s32 *)(gGaxSpeechObject + GAX_SPEECH_INDEX_OFF) = -1;
    *(u32 *)(gGaxSpeechObject + GAX_SPEECH_FRAME_CURSOR_OFF) = 0;
    *(u32 *)(gGaxSpeechObject + GAX_SPEECH_BIT_INDEX_OFF) = 0;

    /* Mirror vanilla post-init rate latch + speech object install. */
    *(void **)(ws + GAX_WS_RATE_OFF) = *(void **)(ws + GAX_WS_RATE_SRC_OFF);
    *(u8 **)(ws + GAX_WS_SPEECH_OBJ_OFF) = gGaxSpeechObject;

    gGaxSpeechProbeCount = 0;
    GaxLogSpeechContract("attach", workspace);
    REG_IME = ime;
}

APPEND_TEXT void GaxBootInit__Replacement(void)
{
    Gax2New(&gGaxParams);
    gGaxParams.debugAssert = 0;
    gGaxParams.wramSize = GAX_MIX_BUFFER_BYTES;
    gGaxParams.wram = gGaxMixBuffer;
    /* Custom audio swaps in the extended package (vanilla FX verbatim + voice
     * entries past slot 123). gax_fx(id) sequences list[id] on an FX channel. */
    gGaxParams.package =
        gRuntimeConfig.custom_gax_audio ? (const void *)gGaxPackageEx : GAX_PACKAGE_ROM;
    gGaxParams.flags = (u16)(gGaxParams.flags & ~GAX_FLAG_SPEECH);

    Gax2Init(&gGaxParams);
    GaxAttachSpeech();
    gGaxVoiceFxChannel = -1;
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

/*
 * Voice playback rides the FX engine: clip N is FX id GAX_VOICE_FX_BASE+N,
 * a 0x44-byte entry (instrument + embedded volenv/perf/wave params) whose
 * wave slot holds unsigned 8-bit PCM (0x80=silence) at the FX mix rate. IRQ-guarded because
 * the mixer walks channel state from IRQ context.
 */
APPEND_TEXT void GaxPlayVoice(u16 id, s16 pan)
{
    u16 ime;
    u8 *state;
    u8 *chArr;
    u8 *ch;
    s32 nCh;
    s32 c;
    s32 best;
    u32 bestPrio;
    u32 prio;

    (void)pan;

    if (!gRuntimeConfig.custom_gax_audio)
        return;
    if (id >= gGaxVoiceCount)
        return;
    if (gGaxVoiceTable[id].data == NULL)
        return;

    ime = REG_IME;
    REG_IME = 0;

    state = (u8 *)gGaxWorkspacePtr;
    if (state == NULL)
    {
        REG_IME = ime;
        return;
    }

    /* Keep FX processing alive even when the current BGM has 0 FX tracks. */
    gGaxParams.flags = (u16)(gGaxParams.flags | 0x0002);
    state[0x59] = 1;

    if (gGaxVoiceFxChannel >= 0)
        StopFxChannel(gGaxVoiceFxChannel);

    /*
     * Drive the FX queue like PlaySfxEx (select free channel, stash id /
     * priority / note). Direct writes avoid depending on song-apply having
     * already latched fxEn for BGMs with music+0xA0 == 0.
     */
    nCh = *(s32 *)(state + 0x10);
    chArr = *(u8 **)(state + 0x0C);
    best = -1;
    bestPrio = 0x7FFFFFFFu;
    for (c = 0; c < nCh; c++)
    {
        prio = *(u32 *)(chArr + c * 0x50 + 0x40);
        if ((s32)prio <= (s32)bestPrio)
        {
            bestPrio = prio;
            best = c;
        }
    }
    if (best < 0)
    {
        REG_IME = ime;
        return;
    }

    ch = chArr + best * 0x50;
    ch[0x4B] = 8; /* note=-1 path in PlaySfxEx */
    ch[0x49] = 0;
    ch[0x4A] = 1;
    ch[0x4C] = (u8)(GAX_VOICE_FX_BASE + id);
    ch[0x48] = 0xFF;
    *(u32 *)(ch + 0x40) = GAX_VOICE_FX_PRIORITY;
    state[0x88 + best] = (u8)(GAX_VOICE_FX_BASE + id);
    state[0x59] = 1;

    gGaxVoiceFxChannel = (s16)best;
    NoCashGBAPrintf("GAX voice fx id=%d ch=%d", id, best);
    REG_IME = ime;
}

APPEND_TEXT void GaxStopVoice(void)
{
    u16 ime;

    if (gGaxVoiceFxChannel < 0)
        return;

    ime = REG_IME;
    REG_IME = 0;
    StopFxChannel(gGaxVoiceFxChannel);
    gGaxVoiceFxChannel = -1;
    NoCashGBAPrintf("GAX voice fx stop");
    REG_IME = ime;
}

/*
 * Temporary consumer probe @ 0x08056A30 — log the first active tick, then run
 * the vanilla body via GaxSpeechConsumer__Continue (stolen prologue trampoline).
 */
APPEND_TEXT s32 GaxSpeechConsumer__Replacement(void *speechObj)
{
    s32 index;
    u8 *state;
    u8 *chArr;

    /* Hold FX enable while a voice clip is queued/playing so song-apply
     * (which clears +0x59 for BGMs with no FX tracks) cannot starve it.
     * Drop the latch when the channel returns to the idle sentinel. */
    if (gGaxVoiceFxChannel >= 0)
    {
        state = (u8 *)gGaxWorkspacePtr;
        if (state != NULL)
        {
            chArr = *(u8 **)(state + 0x0C);
            if (*(u32 *)(chArr + gGaxVoiceFxChannel * 0x50 + 0x40) == 0x80000000u)
                gGaxVoiceFxChannel = -1;
            else
                state[0x59] = 1;
        }
    }

    if (speechObj != NULL && gGaxSpeechProbeCount == 0)
    {
        index = *(s32 *)((u8 *)speechObj + GAX_SPEECH_INDEX_OFF);
        if (index != -1)
        {
            NoCashGBAPrintf(
                "GAX consumer first tick obj=%x idx=%d fc=%u bi=%u",
                (u32)speechObj,
                index,
                *(u32 *)((u8 *)speechObj + GAX_SPEECH_FRAME_CURSOR_OFF),
                *(u32 *)((u8 *)speechObj + GAX_SPEECH_BIT_INDEX_OFF));
            gGaxSpeechProbeCount = 1;
        }
    }

    return GaxSpeechConsumer__Continue(speechObj);
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
    u8 *state;
    u8 *chArr;

    ApplyTalkVoiceCue(stream);

    if (gGaxVoiceFxChannel >= 0)
    {
        state = (u8 *)gGaxWorkspacePtr;
        if (state != NULL)
        {
            chArr = *(u8 **)(state + 0x0C);
            if (*(u32 *)(chArr + gGaxVoiceFxChannel * 0x50 + 0x40) == 0x80000000u)
                gGaxVoiceFxChannel = -1;
            else
                state[0x59] = 1;
        }
    }

    return TalkAdvance(stream);
}

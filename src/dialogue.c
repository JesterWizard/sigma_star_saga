#include "global.h"
#include "dialogue.h"
#include "gax.h"
#include "ram_map.h"

/*
 * Talk bank helpers — decompiled from baserom @ 0x080102BC–0x080109CC.
 *
 * Linked into append (.text). Vanilla ROM still owns the original bytes until
 * a LynJump redirects callers; custom / cutscene peels can call these by name.
 */

typedef void (*TalkHelperFunc)(void);

#define TalkHelper_10248 ((TalkHelperFunc)0x08010249)
#define TalkHelper_10364 ((TalkHelperFunc)0x08010365)

/* EXPR_ALT (expr != 0): optional SFX keyed by speaker id 6..20 (vanilla table). */
static const u8 sTalkExprSfx[15] = {
    0x7A, 0x7B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x85, 0x85, 0x85, 0x85, 0x00, 0x76, 0x75,
    0x83,
};

/* Vanilla @ 0x080102BC — portrait header consume. */
const u8 *TalkAdvance(const u8 *stream)
{
    u8 speaker;
    u8 side;
    u8 expr;
    u8 sfx;

    speaker = stream[1];
    side = stream[2];
    expr = stream[3];

    gTalkExtra = speaker;
    gTalkPortraitSide = side;
    gTalkPortraitExpr = expr;

    stream += 5;

    if (expr == 0)
        return stream;

    if (speaker >= 6 && speaker <= 20)
    {
        sfx = sTalkExprSfx[speaker - 6];
        if (sfx != 0)
            PlaySfx(sfx, 3);
    }

    return stream;
}

void StartTalkPtr(const u8 *stream, u8 paramA, u8 paramB)
{
    u16 flags;

    flags = *(u16 *)(gTalkState + TALK_STATE_FLAGS_OFF);
    *(u16 *)(gTalkState + TALK_STATE_FLAGS_OFF) = flags | TALK_FLAG_BUSY;

    gTalkStreamPtr = stream;
    gTalkParamA = paramA;
    gTalkParamB = paramB;
    gTalkBusyA = 1;
    gTalkBusyB = 1;
    gTalkParamAMirror = paramA;
    gTalkLayout = 18 - paramB;
    gTalkPagePos = 0;
    gTalkPagePos2 = 0;

    if (gTalkLayoutCmp == gTalkLayout)
        TalkHelper_10248();

    gTalkActive = 1;

    if (gTalkStreamPtr[0] == 7)
    {
        gTalkStreamPtr = TalkAdvance(gTalkStreamPtr);
        TalkHelper_10364();
    }

    gTalkGate = 1;
}

void StartTalkById(u16 scriptId, u8 paramA, u8 paramB)
{
    s32 bank;
    s32 i;
    const u32 *ranges;
    const u8 *const *banks;
    const u8 *stream;
    u16 flags;

    bank = -1;
    flags = *(u16 *)(gTalkState + TALK_STATE_FLAGS_OFF);
    *(u16 *)(gTalkState + TALK_STATE_FLAGS_OFF) = flags | TALK_FLAG_BUSY;

    ranges = gDialogueIdRanges;
    if (scriptId >= ranges[0] && scriptId <= ranges[1])
    {
        bank = 0;
    }
    else
    {
        for (i = 1; i <= 6; i++)
        {
            ranges = gDialogueIdRanges + i * 2;
            if (scriptId >= ranges[0] && scriptId <= ranges[1])
            {
                bank = i;
                break;
            }
        }
    }

    if (bank != -1)
    {
        banks = gDialogueBankBases;
        stream = banks[bank] + gDialogueEntryOffsets[scriptId];
        StartTalkPtr(stream, paramA, paramB);
    }
}

void StartTalkByIdEx(u16 scriptId, u8 paramA, u8 paramB, u8 extra)
{
    StartTalkById(scriptId, paramA, paramB);
    gTalkExtra = extra;
}

void BuildTalkOffsets(const u8 *stream, s32 startId, s32 endId)
{
    s32 id;
    u32 byteOff;
    u16 *slot;
    u8 ch;

    id = startId;
    ch = stream[0];
    if (ch == '~' || id > endId)
        return;

    byteOff = 1;
    slot = &gDialogueEntryOffsets[id];

    do
    {
        if (ch == '#')
        {
            *slot = (u16)byteOff;
            slot++;
            id++;
        }
        byteOff++;
        stream++;
        ch = stream[0];
        if (ch == '~')
            break;
    } while (id <= endId);
}

void InitTalkBanks(void)
{
    s32 i;
    const u32 *ranges;
    const u8 *const *banks;

    gDialogueBankBases = gDialogueBankTable;
    ranges = gDialogueIdRanges;

    for (i = 0; i <= 6; i++)
    {
        banks = gDialogueBankBases;
        BuildTalkOffsets(banks[i], (s32)ranges[0], (s32)ranges[1]);
        ranges += 2;
    }
}

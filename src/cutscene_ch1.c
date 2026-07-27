#include "global.h"
#include "actor.h"
#include "cutscene_ch1.h"
#include "dialogue.h"
#include "ram_map.h"

/*
 * CutsceneCh1Opener — decompiled from baserom @ 0x080523EC (span ~0x4E0).
 *
 * Per-frame FSM driven by gCutsceneStep (0–15). Shared trailer at step
 * fallthrough updates camera slots from gCutsceneParam.
 *
 * Linked into append (.text). Vanilla ROM still owns the original bytes until
 * a LynJump redirects the call site; this copy is the named reference peel.
 */

#define KEY_UP   0x0040
#define KEY_DOWN 0x0080

#define ACTOR_POS_X_FROM_FLAGS 0x28 /* → ACTOR_OFF_X */
#define ACTOR_POS_Y_FROM_FLAGS 0x30 /* → ACTOR_OFF_CUT_Y */
#define CAMERA_OFF_X2          0x34

typedef void (*VoidU8U8Func)(u8 a, u8 b);
typedef void (*VoidFunc)(void);
typedef void (*FadeFunc)(u32 a, u32 b, u32 c, u32 d);
typedef s32 (*MulDivFunc)(s32 a, s32 b);
typedef u32 (*RandFunc)(void);

#define Helper_2CD40 ((VoidU8U8Func)0x0802CD41)
#define Helper_318A0 ((VoidU8U8Func)0x080318A1)
#define Helper_52340 ((VoidFunc)0x08052341)
#define Helper_52370 ((VoidFunc)0x08052371)
#define Helper_3B850 ((VoidFunc)0x0803B851)
#define Helper_07348 ((RandFunc)0x08007349)
#define Helper_09070 ((FadeFunc)0x08009071)
#define Helper_589F4 ((MulDivFunc)0x080589F5)

/* Provisional — timer / counter checked in step 0. */
#define gUnk_03003688 (*(vu32 *)0x03003688)
/* Palette / FX block used by step 12 mask. */
#define gUnk_03000BF4 (*(vu16 *)0x03000BF4)

static void CutsceneCh1_ApplyCameraDelta(void)
{
    s32 dx;
    s32 dy;
    s32 t;

    dx = (s32)gCutsceneParam[0];
    dy = (s32)gCutsceneParam[1];

    t = Helper_589F4(dx, 3);
    *(s32 *)(gCameras + 0xB0) += t;
    *(s32 *)(gCameras + 0xB4) += dy;
    *(s32 *)(gCameras + 0x134) += dx; /* 0x9A * 2 */
    *(s32 *)(gCameras + 0x138) += dy; /* 0x9C * 2 */
    t = Helper_589F4(dx, 3);
    *(s32 *)(gCameras + 0x1B8) += t; /* 0xDC * 2 */
    *(s32 *)(gCameras + 0x1BC) += dy; /* 0xDE * 2 */
}

static void CutsceneCh1_NudgeActiveActorsX(s32 delta)
{
    u8 *slot;
    s32 i;

    slot = gActorPool + ACTOR_OFF_FLAGS;
    for (i = 0x27; i >= 0; i--)
    {
        if ((*(u16 *)slot & ACTOR_FLAG_ACTIVE) != 0)
            *(s32 *)(slot + ACTOR_POS_X_FROM_FLAGS) += delta;
        slot += ACTOR_STRIDE;
    }
}

void CutsceneCh1Opener(void)
{
    s32 step;
    s32 i;
    u8 *slot;
    s32 delta;
    u32 keys;

    step = (s32)gCutsceneStep;
    if ((u32)step > 15)
        goto trailer;

    switch (step)
    {
    case 0:
        Helper_2CD40(1, 0);
        Helper_318A0(1, 0);
        if (gUnk_03003688 <= 0x2C)
            break;
        gCutsceneStep = 1;
        gCutsceneParam[0] = 0x40000; /* 0x80 << 11 */
        gCutsceneParam[1] = 0;
        Helper_09070(0, 2, 1, 0);
        break;

    case 1:
        Helper_2CD40(1, 0);
        Helper_318A0(1, 0);
        Helper_52340();
        if ((s32)gWorldScrollY <= (s32)(250 << 18))
            break;
        Helper_2CD40(0, 0);
        Helper_318A0(0, 0);
        Helper_3B850();
        gCutsceneStep = 2;
        break;

    case 2:
        Helper_52340();
        if ((s32)gWorldScrollY > (s32)0x22C40000)
        {
            gCutsceneParam[0] += (s32)0xFFFFFD80;
            if ((s32)gCutsceneParam[0] > 0x7FFF)
                break;
            gCutsceneParam[0] = 0x8000; /* 0x80 << 8 */
            gCutsceneStep = 3;
            break;
        }
        if ((Helper_07348() & 0x1F) != 0)
            break;
        Helper_09070(0, 2, 1, 0);
        break;

    case 3:
        Helper_52370();
        if ((s32)gWorldScrollY > (s32)0x2C600000)
        {
            gCutsceneParam[0] += 0x800; /* 0x80 << 4 */
            if ((s32)gCutsceneParam[0] > (s32)0x40000)
                gCutsceneParam[0] = 0x40000;
        }
        if ((s32)gWorldScrollY <= (s32)0x2EE00000)
            break;
        gCutsceneParam[1] += (s32)0xFFFFFE00;
        if ((s32)gCutsceneParam[1] >= (s32)0xFFFF0000)
            break;
        gCutsceneParam[1] = (u32)0xFFFF0000;
        gCutsceneStep = 6;
        break;

    case 6:
        Helper_52370();
        if ((s32)gWorldScrollX > (s32)0x0E5DFFFF)
            break;
        delta = 0x40000 - (s32)(0x88 << 19);
        gWorldScrollY += (u32)(s32)0xFBC00000;
        *(s32 *)(gCameras + CAMERA_OFF_X) += (s32)0xFBC00000;
        CutsceneCh1_NudgeActiveActorsX(delta);
        gCutsceneParam[0] = 0x8000;
        gCutsceneStep = 5;
        break;

    case 5:
        Helper_52370();
        if ((s32)gWorldScrollX > (s32)0x0D92FFFF)
            break;
        gCutsceneParam[1] += 0x200; /* 0x80 << 2 */
        if ((s32)gCutsceneParam[1] <= 0)
            break;
        gCutsceneParam[1] = 0;
        gCutsceneStep = 10;
        StartTalkById(51, 4, 3); /* scene_05C9E1 */
        break;

    case 10:
        Helper_52370();
        if ((s32)gWorldScrollY <= (s32)0x34260000)
            break;
        delta = (s32)0x0BEE0000;
        gWorldScrollY += (u32)delta;
        *(s32 *)(gCameras + CAMERA_OFF_X) += delta;
        CutsceneCh1_NudgeActiveActorsX(delta);
        gCutsceneStep = 11;
        break;

    case 11:
        Helper_52370();
        Helper_52340();
        if ((s32)gWorldScrollY <= (s32)0x43C60000)
            break;
        gCutsceneParam[1] += 0x200; /* 0x80 << 2 */
        if ((s32)gCutsceneParam[1] <= (s32)0x8000)
            break;
        gCutsceneParam[1] = 0x8000;
        gCutsceneStep = 12;
        gUnk_03000BF4 &= 0xF9FF;
        break;

    case 12:
        Helper_52370();
        if ((s32)gWorldScrollX <= (s32)0x0E380000)
            break;
        delta = (s32)0xF0000000; /* 0xF0 << 24 */
        gWorldScrollY += (u32)delta;
        *(s32 *)(gCameras + CAMERA_OFF_X) += delta;
        *(s32 *)(gCameras + CAMERA_OFF_X2) += delta;

        slot = gActorPool + ACTOR_OFF_FLAGS;
        for (i = 0; i <= 0x27; i++)
        {
            if ((*(u16 *)slot & ACTOR_FLAG_ACTIVE) != 0)
            {
                *(s32 *)(slot + ACTOR_POS_X_FROM_FLAGS) +=
                    (s32)0xFFF80000 - (s32)(0x80 << 21);
                if ((u32)(i - 10) <= 9)
                    *(s32 *)(slot + ACTOR_POS_Y_FROM_FLAGS) += 0x80000; /* 0x80 << 12 */
            }
            slot += ACTOR_STRIDE;
        }
        gCutsceneParam[0] = 0x80000;
        gCutsceneStep = 13;
        break;

    case 13:
        Helper_52370();
        if ((s32)gWorldScrollX <= (s32)0x0F3C0000)
            break;
        gCutsceneParam[1] += (s32)0xFFFFFE00;
        if ((s32)gCutsceneParam[1] >= 0)
            break;
        gCutsceneParam[1] = 0;
        gCutsceneStep = 14;
        break;

    case 14:
        if ((s32)gCutsceneParam[0] > (s32)0x8000)
            gCutsceneParam[0] += (s32)0xFFFFF800;
        Helper_52340();
        break;

    case 15:
        keys = gHeldKeys;
        if ((keys & KEY_UP) != 0)
        {
            gCutsceneParam[1] += (s32)0xFFFFF000;
        }
        else if ((keys & KEY_DOWN) != 0)
        {
            gCutsceneParam[1] += 0x1000; /* 0x80 << 5 */
        }
        Helper_52370();
        break;

    case 4:
    case 7:
    case 8:
    case 9:
    default:
        break;
    }

trailer:
    CutsceneCh1_ApplyCameraDelta();
}

/*
 * CutsceneCh1TargetBrief — decompiled from JT case @ 0x0802BFA0 inside
 * stage update CutsceneStage? @ 0x0802B18C (outer index from 0x03007738).
 *
 * Sub-steps on gCutsceneStep; fires StartTalkById(109) → scene_0605BB.
 * Falls into shared camera lerp @ 0x0802BFCE.
 */
void CutsceneCh1TargetBrief(void)
{
    s32 step;
    s32 dx;
    s32 dy;

    step = (s32)gCutsceneStep;
    if (step == 0)
    {
        gCutsceneStep = 1;
    }
    else if (step == 1)
    {
        if (gUnk_03003688 > 0xB4)
        {
            StartTalkById(109, 4, 3); /* scene_0605BB */
            gCutsceneStep = 2;
        }
    }

    /* Shared lerp: camera +0xB0/+0xB4 += gCutsceneParam >> 1 */
    dx = (s32)gCutsceneParam[0] >> 1;
    dy = (s32)gCutsceneParam[1] >> 1;
    *(s32 *)(gCameras + 0xB0) += dx;
    *(s32 *)(gCameras + 0xB4) += dy;
}

#include "global.h"
#include "actor.h"
#include "cutscene_ch1.h"
#include "cutscene_stage.h"
#include "dialogue.h"
#include "ram_map.h"

/*
 * CutsceneStageUpdate — structural decomp of baserom @ 0x0802B18C (~0xF3C).
 *
 * Outer dispatcher on gStageCase (0..0x10D). Specialty cases are expressed
 * as C state machines. LynJump when .custom_cutscene_stage is TRUE.
 */

#define STAGE_CASE_MAX 0x10D

#define gUnk_03003688 (*(vu32 *)0x03003688)
#define gUnk_03000D80 (*(vu8 *)0x03000D80)
#define gUnk_03007728 ((vu16 *)0x03007728)
#define gUnk_03001638 (*(vu16 *)0x03001638)
#define gUnk_03002774 (*(vu16 *)0x03002774)
#define gUnk_03001F50 (*(vu16 *)0x03001F50)
#define gUnk_03007700 (*(vu8 *)0x03007700)
#define gUnk_03007720 (*(vu8 *)0x03007720)
#define gEnemyRemainingByte (*(vu8 *)0x03007080)

typedef void (*VoidFunc)(void);
typedef void (*VoidU8U8Func)(u8 a, u8 b);
typedef void (*VoidU32Func)(u32 a);
typedef void (*Void3HalfFunc)(u16 a, u16 b, u16 c);
typedef void (*Void4U32Func)(u32 a, u32 b, u32 c, u32 d);
typedef void (*VoidU16Func)(u16 a);
typedef void (*VoidU16U16Func)(u16 a, u16 b);
typedef void (*GetBoundsFunc)(u32 a, u32 b, s16 *left, s16 *top,
                              s16 *right, s16 *bottom);
typedef s32 (*S32U32Func)(u32 a, u32 b);

#define Helper_3BE54 ((VoidFunc)0x0803BE55)
#define Helper_3C1E0 ((VoidFunc)0x0803C1E1)
#define Helper_2F40C ((Void3HalfFunc)0x0802F40D)
#define Helper_2AF68 ((VoidFunc)0x0802AF69)
#define Helper_05B3C ((u32(*)(u32))0x08005B3D)
#define Helper_3B8B8 ((VoidFunc)0x0803B8B9)
#define Helper_2CD40 ((VoidU8U8Func)0x0802CD41)
#define Helper_318A0 ((VoidU8U8Func)0x080318A1)
#define Helper_00390 ((VoidU32Func)0x08000391)
#define Helper_2D040 ((VoidFunc)0x0802D041)
#define Helper_041B4 ((VoidU32Func)0x080041B5)
#define Helper_2ADF0 ((VoidU32Func)0x0802ADF1)
#define Helper_2C024 ((VoidFunc)0x0802C025)
#define Helper_09070 ((Void4U32Func)0x08009071)
#define Helper_589F4 ((S32U32Func)0x080589F5)
#define Helper_07170 ((GetBoundsFunc)0x08007171)
#define Helper_047B0 ((VoidU16U16Func)0x080047B1)
#define Helper_00280 ((VoidU16Func)0x08000281)
#define Helper_00434 ((VoidU32Func)0x08000435)
#define Helper_0047C ((VoidFunc)0x0800047D)
#define Helper_2C13C ((VoidU16Func)0x0802C13D)
#define Helper_523EC ((VoidFunc)0x080523ED)

static s32 *CameraScroll(void)
{
    return (s32 *)(gCameras + 0xB0);
}

static void StageApplyFourCameraDeltas(s32 mainX, s32 mainY,
                                       s32 secondaryX, s32 secondaryY)
{
    s32 *camera = CameraScroll();

    camera[0] += mainX;
    camera[1] += mainY;
    camera[0x21] += secondaryX;
    camera[0x22] += secondaryY;
}

static void StageSharedLerp(void)
{
    s32 dx;
    s32 dy;

    dx = (s32)gCutsceneParam[0] >> 1;
    dy = (s32)gCutsceneParam[1] >> 1;
    *(s32 *)(gCameras + 0xB0) += dx;
    *(s32 *)(gCameras + 0xB4) += dy;
}

static void StageTrailer(void)
{
    s32 sx;
    s32 sy;
    u32 mode;

    *(s32 *)(gCameras + CAMERA_OFF_X) = (s32)gWorldScrollY;
    *(s32 *)(gCameras + CAMERA_OFF_Y) = (s32)gWorldScrollX;

    sx = (s32)gWorldScrollY >> 16;
    sy = (s32)gWorldScrollX >> 16;
    gUnk_03007728[0] = (u16)(sx - 32);
    gUnk_03007728[1] = (u16)(sx + 240);
    gUnk_03007728[2] = (u16)(sy - 32);
    gUnk_03007728[3] = (u16)(sy + 160);

    Helper_2F40C(gUnk_03001638, gUnk_03002774, gUnk_03001F50);

    if (gUnk_03007700 != 0 && gEnemyRemainingByte == 0)
        gStageClearGate = 0;

    if (gStageClearGate != 0 || gUnk_03007720 != 0)
        return;

    gStageClearGate = 1;
    mode = gStageCase;
    if (mode == 0x3E || (mode >= 0x10B && mode <= 0x10C))
        Helper_2AF68();
    else if (Helper_05B3C(0x75) == 0xFF)
        Helper_3B8B8();
}

static void StageCaseBounds(u16 hitMode);

static void StageCaseTalk109(void)
{
    if (gCutsceneStep == 0)
        gCutsceneStep = 1;
    else if (gCutsceneStep == 1 && gUnk_03003688 > 0xB4)
    {
        StartTalkById(109, 4, 3);
        gCutsceneStep = 2;
    }
    StageSharedLerp();
}

static void StageCase175(void)
{
    if (gUnk_03003688 == 0xB4)
        StartTalkById(451, 4, 3);
    StageCaseBounds(0x100);
}

static void StageCase11(void)
{
    s32 *camera = CameraScroll();

    camera[0] += (s32)gCutsceneParam[0];
    camera[1] += (s32)gCutsceneParam[1];
    camera[0x21] += ((s32)gCutsceneParam[0] >> 2) * 3;
    camera[0x22] += (s32)gCutsceneParam[1];

    switch (gCutsceneStep)
    {
    case 0:
        gCutsceneParam[0] = 0x40000;
        gCutsceneParam[1] = 0;
        gCutsceneStep = 1;
        break;
    case 1:
        if (gUnk_03003688 > 0xE8E0000)
        {
            gCutsceneParam[0] += (s32)0xFFFFF400;
            if ((s32)gCutsceneParam[0] <= (s32)0xFFFFF400)
            {
                gCutsceneParam[0] = 0x10000;
                gCutsceneStep = 2;
            }
        }
        break;
    case 2:
        if (gUnk_03003688 > 0x15180000)
        {
            Helper_09070(0, 1, 30, 0);
            camera[1] += 0x8000;
            if ((s32)(gWorldScrollX - camera[1]) > 0x60000000)
                gCutsceneStep = 3;
        }
        break;
    case 3:
        if (gUnk_03003688 > 0x25800000)
        {
            gCutsceneParam[1] += 0xC00;
            if ((s32)gCutsceneParam[1] > ((s32)gCutsceneParam[0] >> 1))
            {
                gCutsceneParam[1] = (s32)gCutsceneParam[0] >> 1;
                gCutsceneStep = 4;
            }
        }
        break;
    case 4:
        if (gUnk_03003688 > 0x44800000)
        {
            gCutsceneParam[1] += (s32)0xFFFFF400;
            if ((s32)gCutsceneParam[1] < 0)
            {
                gCutsceneParam[1] = 0;
                gCutsceneStep = 5;
            }
        }
        break;
    case 5:
        if (gUnk_03003688 > 0x29360000)
        {
            gCutsceneParam[1] += 0xC00;
            if ((s32)gCutsceneParam[1] > (s32)gCutsceneParam[0])
            {
                gCutsceneParam[1] = gCutsceneParam[0];
                gCutsceneStep = 6;
            }
        }
        break;
    case 6:
        if (gUnk_03003688 > 0x4FE0000)
        {
            gCutsceneParam[1] += (s32)0xFFFFF400;
            if ((s32)gCutsceneParam[1] < 0)
            {
                gCutsceneParam[1] = 0;
                gCutsceneStep = 7;
            }
        }
        break;
    case 7:
        if (gUnk_03003688 > 0x29D60000)
        {
            Helper_09070(0, 1, 30, 0);
            camera[1] += (s32)0xFFFF8000;
            if ((s32)(gWorldScrollX - camera[1]) > 0x1540000)
                gCutsceneStep = 8;
        }
        break;
    case 8:
        if (gUnk_03003688 > 0x2CCE0000)
        {
            Helper_09070(0, 1, 30, 0);
            camera[1] += (s32)0xFFFF8000;
            if ((s32)(gWorldScrollX - camera[1]) > 0x28A0000)
                gCutsceneStep = 9;
        }
        break;
    case 9:
        if (gUnk_03003688 > 0x3AFC0000)
        {
            gCutsceneParam[0] += (s32)0xFFFFFF00;
            if ((s32)gCutsceneParam[0] < 0)
            {
                gCutsceneParam[0] = 0;
                gCutsceneStep = 10;
            }
        }
        break;
    default:
        break;
    }
}

static void StageCase62(void)
{
    s32 *camera = CameraScroll();
    const s16 *waveTable = (const s16 *)0x0805A93C;
    u16 *waveOutput = gCutsceneWaveOutput;
    u8 phase;
    s32 i;

    if (gUnk_03000D80 == 0)
    {
        phase = *(u8 *)(gCameras + 0x136);
        for (i = 0; i < 160; i++)
        {
            u8 sample = phase++;
            waveOutput[i] = (sample + (waveTable[sample] >> 5)) & 0xFF;
        }
        gCutsceneWavePhase = phase + 0x61;
    }

    if (gCutsceneStep == 0)
        gCutsceneStep = 1;
    else if (gCutsceneStep == 1 && gUnk_03003688 > 0x1EA20000)
    {
        gCutsceneStep = 2;
        gCutsceneWavePhase = 0;
    }

    camera[0] += (s32)gCutsceneParam[0];
    camera[1] += (s32)waveTable[gCutsceneParam[0] & 0x1FF] << 7;
    camera[0x21] += (s32)gCutsceneParam[0] >> 1;
    camera[0x22] += (s32)gCutsceneParam[1];
    Helper_523EC();
}

static void StageCase113(void)
{
    s32 *camera = CameraScroll();

    switch (gCutsceneStep)
    {
    case 0:
        gCutsceneStep = 1;
        break;
    case 1:
        if (gUnk_03003688 > 0xAFF0000)
        {
            gCutsceneParam[0] += 0x180;
            if ((s32)gCutsceneParam[0] > 0x40000)
            {
                gCutsceneParam[0] = 0x40000;
                gCutsceneStep = 2;
            }
        }
        break;
    case 2:
        if (gUnk_03003688 > 0x1B980000)
        {
            gCutsceneParam[0] += (s32)0xFFFFFE00;
            if ((s32)gCutsceneParam[0] <= 0x7FFF)
            {
                gCutsceneParam[0] = 0x8000;
                gCutsceneStep = 3;
            }
        }
        break;
    case 3:
        if (gUnk_03003688 > (s32)0x3EC00000)
        {
            gCutsceneParam[0] += (s32)0xFFFFFE00;
            if ((s32)gCutsceneParam[0] < 0)
            {
                gCutsceneParam[0] = 0;
                gCutsceneStep = 4;
            }
        }
        break;
    default:
        break;
    }

    StageApplyFourCameraDeltas((s32)gCutsceneParam[0], (s32)gCutsceneParam[1],
                               (s32)gCutsceneParam[0],
                               Helper_589F4(gCutsceneParam[1], 3));
}

static void StageCase134(void)
{
    if (gCutsceneStep == 0)
        gCutsceneStep = 1;
}

static void StageCase196(void)
{
    s32 *camera = CameraScroll();
    const s16 *waveTable = (const s16 *)0x0805A93C;

    switch (gCutsceneStep)
    {
    case 0:
        gCutsceneStep = 1;
        break;
    case 1:
        if (gUnk_03003688 > 0x126C0000)
        {
            gCutsceneParam[0] = 0x8000;
            gCutsceneParam[1] = 0x4000;
            gCutsceneStep = 2;
        }
        break;
    case 2:
        if ((s32)gWorldScrollX > (s32)0xD7000000)
        {
            gWorldScrollX = 0xD7000000;
            gCutsceneParam[0] = 0x8000;
            gCutsceneParam[1] = 0;
            gCutsceneStep = 3;
        }
        break;
    case 3:
        if (gUnk_03003688 > 0x1D3B0000)
        {
            gCutsceneParam[0] += (s32)0xFFFFFE80;
            if ((s32)gCutsceneParam[0] < 0)
            {
                gCutsceneParam[0] = 0;
                gCutsceneStep = 4;
            }
        }
        break;
    default:
        break;
    }

    camera[0] += (s32)gCutsceneParam[0];
    camera[1] += (s32)waveTable[(gCutsceneParam[0] >> 2) & 0xFF] << 7;
    camera[0x21] += (s32)gCutsceneParam[0] >> 1;
    camera[0x22] += (s32)gCutsceneParam[1];
}

static void StageCase249(void)
{
    if (gCutsceneStep == 0)
    {
        if (gUnk_03003688 > 120)
        {
            gCutsceneStep = 1;
            Helper_00280(2);
        }
    }
    else if (gCutsceneStep == 1 && gUnk_03003688 > 300)
    {
        gCutsceneStep = 2;
        Helper_00390(0);
        Helper_00434(0x08138A9C);
    }
}

static void StageCase250(void)
{
    s32 *camera = CameraScroll();

    camera[0] = gWorldScrollY;
    camera[1] = gWorldScrollX;

    if (gCutsceneStep == 0)
    {
        camera[0x21] = camera[0];
        camera[0x22] = camera[1];
        if (gUnk_03003688 > 120)
        {
            gCutsceneStep = 1;
            Helper_00280(2);
        }
    }
    else if (gCutsceneStep == 1)
    {
        camera[0x21] = camera[0];
        camera[0x22] = camera[1];
        if (gUnk_03003688 > 420)
        {
            Helper_0047C();
            Helper_00390(0x100);
            gCutsceneStep = 2;
            Helper_00390(0x100);
            Helper_00434(0x081393DC);
        }
    }
}

static void StageCaseBounds(u16 hitMode)
{
    s16 left;
    s16 top;
    s16 right;
    s16 bottom;
    const u8 *boundsBase;
    u8 index;
    u16 action;

    Helper_07170(0, 0, &left, &top, &right, &bottom);

    index = gEncounterObjIndices[2];
    boundsBase = *(const u8 *const *)0x03006FF0 + index * 16;
    if (left >= *(const u16 *)(boundsBase + 0) &&
        top <= *(const u16 *)(boundsBase + 4) &&
        right >= *(const u16 *)(boundsBase + 2) &&
        bottom <= *(const u16 *)(boundsBase + 6))
    {
        action = *(const u16 *)(boundsBase + 10);
        gUnk_03003688 = 0;
        if (action == 0xFF)
        {
            gCutsceneStep = 0xFF;
            Helper_2C13C(250);
        }
        else
        {
            Helper_2C13C(action + 175);
        }
        Helper_047B0(0x84, hitMode);
    }

    index = gEncounterObjIndices[3];
    boundsBase = *(const u8 *const *)0x03006FF0 + index * 16;
    if (left >= *(const u16 *)(boundsBase + 0) &&
        top <= *(const u16 *)(boundsBase + 4) &&
        right >= *(const u16 *)(boundsBase + 2) &&
        bottom <= *(const u16 *)(boundsBase + 6))
    {
        action = *(const u16 *)(boundsBase + 10);
        gUnk_03003688 = 0;
        if (action == 0xFF)
        {
            gCutsceneStep = 0xFF;
            Helper_2C13C(250);
        }
        else
        {
            Helper_2C13C(action + 175);
        }
        Helper_047B0(0x84, hitMode << 1);
    }

    StageApplyFourCameraDeltas(((s32)gCutsceneParam[0] >> 2) * 5,
                               (s32)gCutsceneParam[1],
                               ((s32)gCutsceneParam[0] >> 2) * 3,
                               (s32)gCutsceneParam[1]);
}

static void StageCaseIntroCtrl(void)
{
    Helper_2CD40(1, 0);
    Helper_318A0(1, 0);
    if ((u32)gCutsceneStep <= 4)
    {
        switch ((s32)gCutsceneStep)
        {
        case 0:
        case 1:
            gCutsceneStep = 1;
            Helper_00390(0x100);
            break;
        case 2:
            if (gUnk_03003688 > 500)
            {
                Helper_2D040();
                gCutsceneStep = 2;
            }
            break;
        case 3:
            if (gUnk_03003688 > 500)
            {
                Helper_041B4(13);
                gCutsceneStep = 3;
            }
            break;
        case 4:
            if (gCutsceneGateHw == 0x1F00)
            {
                gCutsceneStep = 4;
                gUnk_03003688 = 0;
            }
            break;
        }
    }
    *(s32 *)(gCameras + 0xB0) += 0x8000;
}

void CutsceneStageUpdate(void)
{
    u32 stageCase;

    Helper_3BE54();
    if (gUnk_03000D80 != 0)
        Helper_2C024();

    gWorldScrollY += gCutsceneParam[0];
    gWorldScrollX += gCutsceneParam[1];
    Helper_3C1E0();

    stageCase = gStageCase;
    if (stageCase > STAGE_CASE_MAX)
    {
        StageTrailer();
        return;
    }

    switch (stageCase)
    {
    case 0:
        StageCaseTalk109();
        break;

    case 11:
        StageCase11();
        break;

    case 10:
        CutsceneCh1Opener();
        break;

    case 62:
        StageCase62();
        break;

    case 113:
        StageCase113();
        break;

    case 134:
        StageCase134();
        break;

    case 175:
        StageCase175();
        break;

    case 176:
    case 177:
    case 178:
    case 179:
    case 180:
    case 181:
    case 182:
    case 183:
    case 184:
    case 185:
        StageCaseBounds(0x100);
        break;

    case 196:
        StageCase196();
        break;

    case 249:
        StageCase249();
        break;

    case 250:
    case 267:
        StageCase250();
        break;

    case 268:
        StageCaseIntroCtrl();
        break;

    case 269:
        Helper_2ADF0(0);
        break;

    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        /* JT → trailer only */
        break;

    default:
        /* Default JT branch uses the common two-camera lerp. */
        StageSharedLerp();
        break;
    }

    StageTrailer();
}

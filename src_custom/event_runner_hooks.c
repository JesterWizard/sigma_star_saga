#include "global.h"
#include "actor.h"
#include "actor_spawn.h"
#include "cutscene_ch1.h"
#include "dialogue.h"
#include "event_runner.h"
#include "gax_audio.h"
#include "ram_map.h"
#include "runtime.h"

/*
 * Event runner — interprets compile_events.py opcode tables.
 * LynJump CutsceneCh1Opener → CutsceneCh1Opener__Replacement when
 * .custom_event_runner is TRUE (see tools/apply_lynjump.py).
 */

#define gUnk_03003688 (*(vu32 *)0x03003688)
#define gUnk_03000BF4 (*(vu16 *)0x03000BF4)

typedef void (*VoidFunc)(void);
typedef void (*VoidU8U8Func)(u8 a, u8 b);
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

#define KEY_UP   0x0040
#define KEY_DOWN 0x0080
#define ACTOR_POS_X_FROM_FLAGS 0x28
#define ACTOR_POS_Y_FROM_FLAGS 0x30
#define CAMERA_OFF_X2 0x34

LYN_REPLACE_CHECK(CutsceneCh1Opener__Replacement);

static void Runner_ApplyCameraDelta(void)
{
    s32 dx = (s32)gCutsceneParam[0];
    s32 dy = (s32)gCutsceneParam[1];
    s32 t = Helper_589F4(dx, 3);

    *(s32 *)(gCameras + 0xB0) += t;
    *(s32 *)(gCameras + 0xB4) += dy;
    *(s32 *)(gCameras + 0x134) += dx;
    *(s32 *)(gCameras + 0x138) += dy;
    t = Helper_589F4(dx, 3);
    *(s32 *)(gCameras + 0x1B8) += t;
    *(s32 *)(gCameras + 0x1BC) += dy;
}

static void Runner_MoveFlyingActorsX(s32 delta)
{
    u8 *slot = gActorPool + ACTOR_OFF_FLAGS;
    s32 i;

    for (i = 0x27; i >= 0; i--)
    {
        if ((*(u16 *)slot & ACTOR_FLAG_ACTIVE) != 0)
            *(s32 *)(slot + ACTOR_POS_X_FROM_FLAGS) += delta;
        slot += ACTOR_STRIDE;
    }
}

static void Runner_MoveFlyingActorsXY(s32 dx, s32 dySlots)
{
    u8 *slot = gActorPool + ACTOR_OFF_FLAGS;
    s32 i;

    for (i = 0; i <= 0x27; i++)
    {
        if ((*(u16 *)slot & ACTOR_FLAG_ACTIVE) != 0)
        {
            *(s32 *)(slot + ACTOR_POS_X_FROM_FLAGS) += dx;
            if ((u32)(i - 10) <= 9)
                *(s32 *)(slot + ACTOR_POS_Y_FROM_FLAGS) += dySlots;
        }
        slot += ACTOR_STRIDE;
    }
}

static int StrEq(const char *a, const char *b)
{
    while (*a && *a == *b)
    {
        a++;
        b++;
    }
    return *a == *b;
}

static const EventScript *FindScript(const char *name)
{
    u32 i;

    for (i = 0; i < gEventScriptCount; i++)
    {
        if (StrEq(gEventScriptTable[i].name, name))
            return &gEventScriptTable[i];
    }
    return NULL;
}

static u16 SegmentStart(const EventScript *script, u32 step)
{
    u16 i;
    u32 marked = 0;
    const EventOp *ops = script->ops;

    /* Step 0 = ops before the first SET_STEP/MARK. Later steps begin after
     * SET_STEP(n) / MARK_STEP(n) matching `step`. */
    if (step == 0)
        return 0;

    for (i = 0; i < script->opCount; i++)
    {
        if ((ops[i].op == EVOP_SET_STEP || ops[i].op == EVOP_MARK_STEP)
            && (u32)ops[i].arg0 == step)
            return (u16)(i + 1);
        if (ops[i].op == EVOP_SET_STEP || ops[i].op == EVOP_MARK_STEP)
            marked++;
    }
    return 0xFFFF;
}

static u8 Runner_ExecSegment(const EventScript *script, u16 start)
{
    u16 i;
    const EventOp *op;
    u32 keys;

    if (start == 0xFFFF)
        return 0;

    for (i = start; i < script->opCount; i++)
    {
        op = &script->ops[i];
        switch (op->op)
        {
        case EVOP_END:
            return 1;

        case EVOP_SET_STEP:
        case EVOP_MARK_STEP:
            /* Boundary — stop this frame's segment after applying. */
            gCutsceneStep = (u32)op->arg0;
            return 1;

        case EVOP_WAIT_COUNTER_GT:
            if (gUnk_03003688 <= (u32)op->arg0)
                return 1;
            break;

        case EVOP_WAIT_SCROLL_Y_GT:
            if ((s32)gWorldScrollY <= op->arg0)
                return 1;
            break;

        case EVOP_WAIT_SCROLL_Y_LE:
            if ((s32)gWorldScrollY > op->arg0)
                return 1;
            break;

        case EVOP_WAIT_SCROLL_X_GT:
            if ((s32)gWorldScrollX <= op->arg0)
                return 1;
            break;

        case EVOP_WAIT_SCROLL_X_LE:
            if ((s32)gWorldScrollX > op->arg0)
                return 1;
            break;

        case EVOP_WAIT_VEL_GT:
            if ((s32)gCutsceneParam[op->arg0] <= op->arg1)
                return 1;
            break;

        case EVOP_WAIT_VEL_LE:
            if ((s32)gCutsceneParam[op->arg0] > op->arg1)
                return 1;
            break;

        case EVOP_WAIT_RAND_CLEAR:
            if ((Helper_07348() & (u32)op->arg0) != 0)
                return 1;
            break;

        case EVOP_SET_SCROLL_VEL:
            gCutsceneParam[0] = (u32)op->arg0;
            gCutsceneParam[1] = (u32)op->arg1;
            break;

        case EVOP_SET_SCROLL_VEL_X:
            gCutsceneParam[0] = (u32)op->arg0;
            break;

        case EVOP_SET_SCROLL_VEL_Y:
            gCutsceneParam[1] = (u32)op->arg0;
            break;

        case EVOP_ADD_SCROLL_VEL_X:
            gCutsceneParam[0] += (u32)op->arg0;
            break;

        case EVOP_ADD_SCROLL_VEL_Y:
            gCutsceneParam[1] += (u32)op->arg0;
            break;

        case EVOP_CLAMP_SCROLL_VEL_X:
            gCutsceneParam[0] = (u32)op->arg0;
            break;

        case EVOP_CLAMP_SCROLL_VEL_Y:
            gCutsceneParam[1] = (u32)op->arg0;
            break;

        case EVOP_SCROLL_WORLD_Y:
            gWorldScrollY += (u32)op->arg0;
            *(s32 *)(gCameras + CAMERA_OFF_X) += op->arg0;
            break;

        case EVOP_SCROLL_WORLD_Y_CAM2:
            gWorldScrollY += (u32)op->arg0;
            *(s32 *)(gCameras + CAMERA_OFF_X) += op->arg0;
            *(s32 *)(gCameras + CAMERA_OFF_X2) += op->arg0;
            break;

        case EVOP_MOVE_FLYING_ACTORS_X:
            Runner_MoveFlyingActorsX(op->arg0);
            break;

        case EVOP_MOVE_FLYING_ACTORS_XY:
            Runner_MoveFlyingActorsXY(op->arg0, op->arg1);
            break;

        case EVOP_SET_ACTOR_CTRL:
            Helper_2CD40((u8)op->arg0, 0);
            Helper_318A0((u8)op->arg0, 0);
            break;

        case EVOP_TICK_A:
            Helper_52340();
            break;

        case EVOP_TICK_B:
            Helper_52370();
            break;

        case EVOP_FX:
            Helper_3B850();
            break;

        case EVOP_FADE:
            Helper_09070(
                (u32)op->arg0 & 0xFF,
                ((u32)op->arg0 >> 8) & 0xFF,
                ((u32)op->arg0 >> 16) & 0xFF,
                ((u32)op->arg0 >> 24) & 0xFF);
            break;

        case EVOP_PALETTE_AND:
            gUnk_03000BF4 &= (u16)op->arg0;
            break;

        case EVOP_SCROLL_VEL_FROM_DPAD:
            keys = gHeldKeys;
            if ((keys & KEY_UP) != 0)
                gCutsceneParam[1] += (u32)(s32)0xFFFFF000;
            else if ((keys & KEY_DOWN) != 0)
                gCutsceneParam[1] += 0x1000;
            break;

        case EVOP_APPLY_CAMERA_DELTA:
            Runner_ApplyCameraDelta();
            break;

        case EVOP_TALK_BY_ID:
            StartTalkById(
                (u16)op->arg0,
                (u8)(op->arg1 & 0xFF),
                (u8)((op->arg1 >> 8) & 0xFF));
            break;

        case EVOP_SHOW_ACTOR:
            ShowActor(
                (u8)op->arg0,
                (u16)(op->arg1 & 0xFFFF),
                (u16)((op->arg1 >> 16) & 0xFFFF));
            break;

        case EVOP_HIDE_ACTOR:
            HideActor((u8)op->arg0);
            break;

        case EVOP_DELETE_ACTOR:
            DeleteActor((u8)op->arg0);
            break;

        case EVOP_SPAWN_ACTOR:
            SpawnActor(
                (u8)(op->arg0 & 0xFF),
                (u16)((op->arg0 >> 8) & 0xFFFF),
                (u16)((op->arg0 >> 24) & 0xFF),
                (u16)(op->arg1 & 0xFFFF),
                (u16)((op->arg1 >> 16) & 0xFFFF),
                0,
                SPAWN_ACTOR_DEFAULT_FLAGS,
                SPAWN_ACTOR_FIELD1E_NONE);
            break;

        case EVOP_SET_ACTOR_VEL_X:
            SetActorVelX((u8)op->arg0, op->arg1);
            break;

        case EVOP_SET_ACTOR_VEL_Y:
            SetActorVelY((u8)op->arg0, op->arg1);
            break;

        case EVOP_WALK_ACTOR_DIR:
            WalkActorDir(
                (u8)(op->arg0 & 0xFF),
                (u8)((op->arg0 >> 8) & 0xFF),
                op->arg1);
            break;

        case EVOP_MOVE_ACTOR:
            MoveActor(
                (u8)op->arg0,
                (u16)(op->arg1 & 0xFFFF),
                (u16)((op->arg1 >> 16) & 0xFFFF));
            break;

        case EVOP_WALK_ACTOR:
            WalkActor(
                (u8)(op->arg0 & 0xFF),
                (u16)(op->arg1 & 0xFFFF),
                (u16)((op->arg1 >> 16) & 0xFFFF),
                op->arg0 & ~0xFF);
            break;

        case EVOP_PLAY_BGM:
            GaxPlayMusic((u16)op->arg0);
            break;

        case EVOP_STOP_BGM:
            GaxStopMusic();
            break;

        case EVOP_PLAY_VOICE:
            GaxPlayVoice((u16)op->arg0, (s16)op->arg1);
            break;

        case EVOP_STOP_VOICE:
            GaxStopVoice();
            break;

        default:
            break;
        }
    }
    return 1;
}

APPEND_TEXT u8 EventRunner_TickByName(const char *name)
{
    const EventScript *script = FindScript(name);

    if (script == NULL)
        return 0;
    Runner_ExecSegment(script, SegmentStart(script, gCutsceneStep));
    return 1;
}

APPEND_TEXT void CutsceneCh1Opener__Replacement(void)
{
    if (gRuntimeConfig.custom_event_runner
        && EventRunner_TickByName("scene_05C9E1"))
    {
        Runner_ApplyCameraDelta();
        return;
    }
    CutsceneCh1Opener();
}

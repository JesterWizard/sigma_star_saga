#ifndef GUARD_EVENT_RUNNER_H
#define GUARD_EVENT_RUNNER_H

#include "gba/types.h"

/* Opcodes emitted by tools/compile_events.py */
enum {
    EVOP_END = 0,
    EVOP_SET_STEP = 1,          /* arg0 = step */
    EVOP_WAIT_COUNTER_GT = 2,   /* arg0 = threshold; yield while <= */
    EVOP_WAIT_SCROLL_Y_GT = 3,
    EVOP_WAIT_SCROLL_Y_LE = 4,
    EVOP_WAIT_SCROLL_X_GT = 5,
    EVOP_WAIT_SCROLL_X_LE = 6,
    EVOP_WAIT_VEL_GT = 7,       /* arg0 = index 0/1, arg1 = threshold */
    EVOP_WAIT_VEL_LE = 8,
    EVOP_SET_SCROLL_VEL = 9,    /* arg0 = dx, arg1 = dy */
    EVOP_SET_SCROLL_VEL_X = 10,
    EVOP_SET_SCROLL_VEL_Y = 11,
    EVOP_ADD_SCROLL_VEL_X = 12,
    EVOP_ADD_SCROLL_VEL_Y = 13,
    EVOP_CLAMP_SCROLL_VEL_X = 14,
    EVOP_CLAMP_SCROLL_VEL_Y = 15,
    EVOP_SCROLL_WORLD_Y = 16,
    EVOP_SCROLL_WORLD_Y_CAM2 = 17,
    EVOP_MOVE_FLYING_ACTORS_X = 18,
    EVOP_MOVE_FLYING_ACTORS_XY = 19,
    EVOP_SET_ACTOR_CTRL = 20,   /* arg0 = enable */
    EVOP_TICK_A = 21,
    EVOP_TICK_B = 22,
    EVOP_FX = 23,
    EVOP_FADE = 24,             /* arg0 packed: a|b<<8|c<<16|d<<24 */
    EVOP_PALETTE_AND = 25,
    EVOP_WAIT_RAND_CLEAR = 26,  /* arg0 = mask; yield while (rand&mask)!=0 */
    EVOP_SCROLL_VEL_FROM_DPAD = 27,
    EVOP_APPLY_CAMERA_DELTA = 28,
    EVOP_TALK_BY_ID = 29,       /* arg0 = id, arg1 = (a)|(b<<8) */
    EVOP_MARK_STEP = 30,        /* documentary boundary; same as SET_STEP */
    EVOP_SHOW_ACTOR = 31,       /* arg0 = index, arg1 = x|(y<<16) */
    EVOP_HIDE_ACTOR = 32,       /* arg0 = index — clear ACTOR_FLAG_ACTIVE */
    EVOP_DELETE_ACTOR = 33,     /* arg0 = index — DeleteActor */
    EVOP_SPAWN_ACTOR = 34,      /* arg0 packed camera/anim/type; arg1 x|y */
    EVOP_SET_ACTOR_VEL_X = 35,  /* arg0 = index, arg1 = vx */
    EVOP_SET_ACTOR_VEL_Y = 36,
    EVOP_WALK_ACTOR_DIR = 37,   /* arg0 = index|(dir<<8), arg1 = speed */
    EVOP_MOVE_ACTOR = 38,       /* arg0 = index, arg1 = x|(y<<16) */
    EVOP_WALK_ACTOR = 39,       /* arg0 = index|(speed packed), arg1 = x|y */
    EVOP_PLAY_BGM = 40,         /* arg0 = catalog music id */
    EVOP_STOP_BGM = 41,
    EVOP_PLAY_VOICE = 42,       /* arg0 = catalog voice id, arg1 = pan */
    EVOP_STOP_VOICE = 43,
};

typedef struct {
    u32 op;
    s32 arg0;
    s32 arg1;
} EventOp;

typedef struct {
    const char *name;
    u16 talkId;
    u16 opCount;
    const EventOp *ops;
} EventScript;

/* Append tables from compile_events.py */
extern const u32 gEventScriptCount;
extern const EventScript gEventScriptTable[];

/* Per-frame: interpret script for current gCutsceneStep. Returns 1 if found. */
u8 EventRunner_TickByName(const char *name);

/* LynJump body when .custom_event_runner is TRUE. */
void CutsceneCh1Opener__Replacement(void);

#endif /* GUARD_EVENT_RUNNER_H */

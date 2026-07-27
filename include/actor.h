#ifndef GUARD_ACTOR_H
#define GUARD_ACTOR_H

#include "gba/types.h"

#define ACTOR_COUNT  0x28
#define ACTOR_STRIDE 0x60

#define ACTOR_OFF_CAMERA   0x01
#define ACTOR_OFF_KIND     0x04
#define ACTOR_OFF_CLASS    0x11
#define ACTOR_OFF_FLAGS    0x18
#define ACTOR_OFF_ANIM     0x20
#define ACTOR_OFF_DRAW_OX  0x30
#define ACTOR_OFF_DRAW_OY  0x32
#define ACTOR_OFF_HP       0x34
#define ACTOR_OFF_PARAM38  0x38
#define ACTOR_OFF_EXP      0x3C
#define ACTOR_OFF_X        0x40 /* world X (16.16); cutscenes: flags+0x28 */
#define ACTOR_OFF_VX       0x48 /* velocity X (16.16); flight integrate */
#define ACTOR_OFF_CUT_Y    0x48 /* alias: Ch.1 opener nudges slots 10–19 here */
#define ACTOR_OFF_Y        0x4C /* world / draw Y (16.16) */
#define ACTOR_OFF_VY       0x54 /* velocity Y (16.16); flight integrate */
#define ACTOR_OFF_DRAW_NEXT 0x5C

#define ACTOR_FLAG_ACTIVE  0x8000 /* visible / in-use; cutscenes skip if clear */
#define ACTOR_KIND_LAYER_MASK 0xF0
#define ACTOR_KIND_YSORT_EXCL 0xC0

/* Class byte written at spawn: shots @ 0x0802F6C8 / 0x0802F818, enemies as
 * matched by the vanilla nearest-target search @ 0x0802EF1A. */
#define ACTOR_CLASS_PLAYER_SHOT 3
#define ACTOR_CLASS_ENEMY       5

#define CAMERA_STRIDE 0x84
#define CAMERA_OFF_X  0x2C
#define CAMERA_OFF_Y  0x30

/* Vanilla @ 0x080318B4 — sets HP / award / EXP pool / draw layer. */
void InitActorParams(u8 index, s32 hp, s32 param38, s32 expPool, u8 kind);

/* Vanilla @ 0x08006C0C — walk actor pool and emit soft OAM via DrawActor. */
void DrawActors(void);

/* Screen-space position used by DrawActor (world 16.16 − camera + draw nudge). */
void GetActorScreenPos(const u8 *actor, s32 *outX, s32 *outY);

#endif /* GUARD_ACTOR_H */

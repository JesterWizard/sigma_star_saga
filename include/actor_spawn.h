#ifndef GUARD_ACTOR_SPAWN_H
#define GUARD_ACTOR_SPAWN_H

#include "gba/types.h"

/* Cardinal walk dirs for WalkActorDir (Ch.1 velocity vocabulary). */
#define ACTOR_DIR_RIGHT 0
#define ACTOR_DIR_DOWN  1
#define ACTOR_DIR_LEFT  2
#define ACTOR_DIR_UP    3

/* Default flagBits for SPAWN_ACTOR event ops (includes ACTOR_FLAG_ACTIVE). */
#define SPAWN_ACTOR_DEFAULT_FLAGS 0xA102
#define SPAWN_ACTOR_FIELD1E_NONE  0xFFFE

/* Vanilla @ 0x08006178 — allocate pool slot, set ACTIVE, place at (x,y).
 * Returns slot index, or 0xFF on failure. unusedArg is ignored by vanilla. */
u8 SpawnActor(u8 camera, u16 anim, u16 typeId, u16 x, u16 y, u16 unusedArg,
              u16 flagBits, u16 field1e);

/* Vanilla @ 0x08006310 — free gfx and clear flags (remove from scene). */
void DeleteActor(u8 index);

/* Soft visibility: OR / clear ACTOR_FLAG_ACTIVE without freeing gfx.
 * ShowActor also places the actor at (x, y) — same u16 units as SpawnActor. */
void ShowActor(u8 index, u16 x, u16 y);
void HideActor(u8 index);

/* Absolute place at (x, y) (u16 → 16.16 world). */
void MoveActor(u8 index, u16 x, u16 y);

/* Per-slot velocity helpers (16.16). WalkActor sets VX/VY toward (x, y). */
void SetActorVelX(u8 index, s32 vx);
void SetActorVelY(u8 index, s32 vy);
void WalkActor(u8 index, u16 x, u16 y, s32 speed);
void WalkActorDir(u8 index, u8 dir, s32 speed);

#endif /* GUARD_ACTOR_SPAWN_H */

#ifndef GUARD_ACTOR_DEATH_H
#define GUARD_ACTOR_DEATH_H

#include "gba/types.h"

/* Vanilla @ 0x080319B0 — delete actor, optional FX, spawn gem (or skip if
 * gemType == 0xFF). Flight kill path; overworld fauna use PlayerDeathFx
 * @ 0x0801BA4C instead. */
void ActorDeathAward(u8 index, u32 gemType, s32 expPool);

#endif /* GUARD_ACTOR_DEATH_H */

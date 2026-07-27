#ifndef GUARD_DAMAGE_H
#define GUARD_DAMAGE_H

#include "gba/types.h"

/* Vanilla @ 0x08005350 — subtract HP and OR flags into actor+0x18. */
bool8 DamageApply(u8 index, s32 damage, u16 flags);

#endif /* GUARD_DAMAGE_H */

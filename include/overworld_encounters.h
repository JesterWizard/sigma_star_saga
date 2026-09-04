#ifndef GUARD_OVERWORLD_ENCOUNTERS_H
#define GUARD_OVERWORLD_ENCOUNTERS_H

#include "gba/types.h"

/*
 * ScanEncounters — vanilla @ 0x080146B4.
 *
 * mode 0: walk-into map objects (lure circles)
 * mode 1: auto-start on state transitions for type-1 objects
 * mode 2: A-held interaction with type-2 objects (+ nearby-actor probe)
 *
 * Returns non-zero if a battle was started (or mode-2 nearby probe hit).
 * Not the CodeBreaker "random battles" path — that is TryStartRandomBattle.
 */
u8 ScanEncounters(u8 mode);

/* TryStartBattle — vanilla @ 0x08014828. */
u8 TryStartBattle(u16 battleId);

/*
 * TryStartRandomBattle — vanilla @ 0x0801DA5C.
 * Step RNG encounters gated by gRandomBattleCooldown @ 0x03007684.
 */
void TryStartRandomBattle(void);

/* Runtime gate: gDebugMenuToggleRandomBattlesOff (EWRAM; not vanilla cooldown). */
bool8 RandomBattlesDisabled(void);
void RandomBattlesSetDisabled(bool8 disabled);

/* Seeds gDebugMenuToggleRandomBattlesOff from .disable_random_battles (once). */
void RandomBattle_EnsureInit(void);

/* Clear failed vanilla prep after walk epilogue @ 0x8020494. */
void RandomBattle_RecoverOverworldIfStuck(void);

/* Clear trap state 0x39 before the walk collision box @ 0x1F030. */
void RandomBattle_RecoverTrapBeforeWalk(void);

#endif /* GUARD_OVERWORLD_ENCOUNTERS_H */

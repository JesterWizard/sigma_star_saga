#include "global.h"
#include "runtime.h"
#include "overworld_encounters.h"

/*
 * Disable true overworld random battles (TryStartRandomBattle @ 0x0801DA5C).
 *
 * CodeBreaker "no random battles" (with master 9377…):
 *   B581C8AEE38E  →  3aaaaaaa 00yy write 0x01 @ 0x03007685
 * That is byte1 of gRandomBattleCooldown @ 0x03007684. While the word is
 * non-zero, 0x1DA5C decrements it and returns without starting a battle.
 *
 * Live gate: tools/apply_lynjump.py patches 0x1DA5C to `bx lr` when
 * .disable_random_battles is TRUE. Lure circles (ScanEncounters) stay vanilla.
 */

u8 ScanEncounters__Continue(u8 mode);

/* Reference LynJump body (unused while in-place patch is active). */
APPEND_TEXT u8 ScanEncounters__Replacement(u8 mode)
{
    return ScanEncounters__Continue(mode);
}

#include "global.h"
#include "runtime.h"
#include "ram_map.h"
#include "overworld_encounters.h"

/*
 * Disable / gate overworld step random battles (TryStartRandomBattle @
 * 0x0801DA5C). The debug-menu state lives in EWRAM and is seeded from
 * .disable_random_battles.
 *
 * Keep this hook deliberately narrow. Earlier revisions also hooked
 * 0x0801DBB8 and rewrote 0x030076E0 plus player state 0x39, but disassembly
 * shows those belong to unrelated field-transition logic. Touching them when
 * the menu toggle changed is what locked player movement.
 */

void TryStartRandomBattle__Continue(void);

void RandomBattle_EnsureInit(void)
{
    if (gRandomBattleInit)
        return;
    gRandomBattleInit = 1;
    if (gRuntimeConfig.disable_random_battles)
        gDebugMenuToggleRandomBattlesOff = 1;
}

bool8 RandomBattlesDisabled(void)
{
    RandomBattle_EnsureInit();
    return gDebugMenuToggleRandomBattlesOff != 0;
}

void RandomBattlesSetDisabled(bool8 disabled)
{
    RandomBattle_EnsureInit();
    gDebugMenuToggleRandomBattlesOff = disabled ? 1 : 0;
}

APPEND_TEXT void TryStartRandomBattle__Replacement(void)
{
    RandomBattle_EnsureInit();
    if (gDebugMenuToggleRandomBattlesOff != 0)
        return;

    TryStartRandomBattle__Continue();
}

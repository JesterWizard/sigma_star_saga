#include "global.h"
#include "runtime.h"
#include "ram_map.h"
#include "debug_menu.h"

/*
 * Live debug-menu toggles for gameplay cheats that are normally build-time
 * only (gRuntimeConfig is const ROM data). Each toggle is an EWRAM shadow
 * flag seeded once from the RuntimeConfig default, then flipped in place by
 * the debug menu — same EWRAM-latch pattern as gDebugMenuToggleRandomBattlesOff
 * in src_custom/random_battle_hooks.c, but these are the sole runtime source
 * of truth (not ANDed with gRuntimeConfig.X at the call site): flipping one
 * on in the menu must work even when the ROM shipped with that cheat off.
 *
 * all_key_items / all_tools are one-shot latch writes (OR'd into persistent
 * flags every frame while enabled) — turning the toggle back on re-applies
 * them, but turning it off does not revoke items already granted.
 */

void DebugToggles_EnsureInit(void)
{
    if (gDebugTogglesInit)
        return;
    gDebugTogglesInit = 1;

    gDebugMenuToggleMaxHealthOn = gRuntimeConfig.always_max_health;
    gDebugMenuToggleMaxBombsOn = gRuntimeConfig.always_max_bombs;
    gDebugMenuToggleAllItemsOn = (bool8)(gRuntimeConfig.all_key_items || gRuntimeConfig.all_tools);
    gDebugMenuToggleHpBarsOn = gRuntimeConfig.enemy_hp_bars;
}

bool8 DebugToggle_MaxHealthEnabled(void)
{
    DebugToggles_EnsureInit();
    return gDebugMenuToggleMaxHealthOn != 0;
}

void DebugToggle_SetMaxHealth(bool8 enabled)
{
    DebugToggles_EnsureInit();
    gDebugMenuToggleMaxHealthOn = enabled ? 1 : 0;
}

bool8 DebugToggle_MaxBombsEnabled(void)
{
    DebugToggles_EnsureInit();
    return gDebugMenuToggleMaxBombsOn != 0;
}

void DebugToggle_SetMaxBombs(bool8 enabled)
{
    DebugToggles_EnsureInit();
    gDebugMenuToggleMaxBombsOn = enabled ? 1 : 0;
}

bool8 DebugToggle_AllItemsEnabled(void)
{
    DebugToggles_EnsureInit();
    return gDebugMenuToggleAllItemsOn != 0;
}

void DebugToggle_SetAllItems(bool8 enabled)
{
    DebugToggles_EnsureInit();
    gDebugMenuToggleAllItemsOn = enabled ? 1 : 0;
}

bool8 DebugToggle_HpBarsEnabled(void)
{
    DebugToggles_EnsureInit();
    return gDebugMenuToggleHpBarsOn != 0;
}

void DebugToggle_SetHpBars(bool8 enabled)
{
    DebugToggles_EnsureInit();
    gDebugMenuToggleHpBarsOn = enabled ? 1 : 0;
}

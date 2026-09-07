#ifndef GUARD_DEBUG_MENU_H
#define GUARD_DEBUG_MENU_H

#include "gba/types.h"

bool8 DebugMenu_IsBlocking(void);
bool8 DebugMenu_OnOverworldFrame(void);

void DebugToggles_EnsureInit(void);
bool8 DebugToggle_MaxHealthEnabled(void);
void DebugToggle_SetMaxHealth(bool8 enabled);
bool8 DebugToggle_MaxBombsEnabled(void);
void DebugToggle_SetMaxBombs(bool8 enabled);
bool8 DebugToggle_AllItemsEnabled(void);
void DebugToggle_SetAllItems(bool8 enabled);
bool8 DebugToggle_HpBarsEnabled(void);
void DebugToggle_SetHpBars(bool8 enabled);

#endif /* GUARD_DEBUG_MENU_H */

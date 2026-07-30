#ifndef GUARD_LEVEL_UP_H
#define GUARD_LEVEL_UP_H

#include "global.h"
#include "ram_map.h"

#define MAX_PLAYER_LEVEL 99
#define MAX_PLAYER_LEVEL_EXTENDED 255

void CalcExpToNextLevel(void);
bool8 AddExperience(u32 amount);
bool8 AddExperience__Replacement(u32 amount);

#endif // GUARD_LEVEL_UP_H

#ifndef GUARD_LEVEL_UP_H
#define GUARD_LEVEL_UP_H

#include "global.h"

#define MAX_PLAYER_LEVEL 99

extern u8 gPlayerLevel;
extern u32 gPlayerExp;
extern u32 gExpToNextLevel;
extern u32 gExpToCurrentLevel;

void CalcExpToNextLevel(void);
bool8 AddExperience(u32 amount);

#endif // GUARD_LEVEL_UP_H

#include "global.h"
#include "level_up.h"

#define EXP_STEP_BASE 0xC3

void CalcExpToNextLevel(void)
{
    register u32 *r2 asm("r2");
    register u32 r0 asm("r0");
    register u32 r1 asm("r1");
    register u32 *r3 asm("r3");
    register u8 *r4 asm("r4");
    register u32 *r5 asm("r5");

    r2 = &gExpToNextLevel;
    r0 = 0;
    *r2 = r0;
    r1 = 0;
    r4 = &gPlayerLevel;
    r0 = *r4 + 1;
    if (r1 < r0)
    {
        r5 = &gExpToCurrentLevel;
        r3 = r2;
        // Original reuses r2 as the running step after copying the pointer to r3.
        r2 = (u32 *)EXP_STEP_BASE;
        do
        {
            r0 = *r3;
            *r5 = r0;
            r0 = r0 + (u32)r2;
            *r3 = r0;
            r2 = (u32 *)((u32)r2 + EXP_STEP_BASE);
            r1 = r1 + 1;
            r0 = *r4 + 1;
        } while (r1 < r0);
    }
}

bool8 AddExperience(u32 amount)
{
    u8 level = gPlayerLevel;

    if (level > MAX_PLAYER_LEVEL - 1)
        return FALSE;

    gPlayerExp += amount;

    if (level == MAX_PLAYER_LEVEL)
    {
        if (gPlayerExp > gExpToNextLevel)
            gPlayerExp = gExpToNextLevel;
    }

    if (gPlayerExp >= gExpToNextLevel)
    {
        gPlayerLevel++;
        if (gPlayerLevel > MAX_PLAYER_LEVEL - 1)
            gPlayerLevel = MAX_PLAYER_LEVEL;
        CalcExpToNextLevel();
        return TRUE;
    }

    return FALSE;
}

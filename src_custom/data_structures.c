#include "global.h"
#include "runtime.h"
#include "data_structures.h"

APPEND_TEXT u32 RemapEnemyExpAmount(u32 vanilla_amount)
{
    u16 i;
    u16 count = gEnemyExpRemapCount;

    for (i = 0; i < count; i++)
    {
        if (gEnemyExpRemap[i].vanilla == (u16)vanilla_amount)
            return gEnemyExpRemap[i].exp;
    }
    return vanilla_amount;
}

APPEND_TEXT u32 LookupEnemyExpById(u16 enemy_id, u32 vanilla_amount)
{
    u16 lo = 0;
    u16 hi = gEnemyExpByIdCount;

    while (lo < hi)
    {
        u16 mid = (u16)(lo + (hi - lo) / 2);
        u16 id = gEnemyExpById[mid].id;

        if (id == enemy_id)
            return gEnemyExpById[mid].exp;
        if (id < enemy_id)
            lo = (u16)(mid + 1);
        else
            hi = mid;
    }
    return vanilla_amount;
}

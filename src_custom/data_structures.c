#include "global.h"
#include "runtime.h"
#include "ram_map.h"
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

APPEND_TEXT u32 LookupOverworldEnemyExp(u16 enemy_id, u16 anim)
{
    u16 i;
    u16 count = gOverworldEnemyExpCount;
    u32 by_anim = 0;

    for (i = 0; i < count; i++)
    {
        u16 id = gOverworldEnemyExp[i].id;
        u16 a = gOverworldEnemyExp[i].anim;
        u16 exp = gOverworldEnemyExp[i].exp;

        if (id == enemy_id && a == anim)
            return exp;
        /* Anim-only fallback for fauna models (avoids projectile type collisions). */
        if (by_anim == 0 && a == anim && anim != 0)
            by_anim = exp;
    }

    return by_anim;
}

APPEND_TEXT u8 RemapCustomImpactIcon(u8 type, u8 local)
{
    u16 i;

    if (type != 2)
        return local;

    for (i = 0; i < gCustomImpactCount; i++)
    {
        if (gCustomImpacts[i].index == local)
            return gCustomImpacts[i].icon_from;
    }
    return local;
}

APPEND_TEXT void EnsureCustomCannonsOwned(void)
{
    u16 i;

    if (!gRuntimeConfig.custom_gun_data)
        return;

    for (i = 0; i < gCustomCannonCount; i++)
    {
        u8 index = gCustomCannons[i].index;
        u8 id = gCustomCannons[i].id;

        gCannonOwned |= ((u32)1 << index);
        gGunDataBits[id >> 5] |= ((u32)1 << (id & 31));
    }
}

APPEND_TEXT void EnsureCustomBulletsOwned(void)
{
    u16 i;

    if (!gRuntimeConfig.custom_gun_data)
        return;

    for (i = 0; i < gCustomBulletCount; i++)
    {
        u8 index = gCustomBullets[i].index;
        u8 id = gCustomBullets[i].id;

        gBulletOwned |= ((u32)1 << index);
        gGunDataBits[id >> 5] |= ((u32)1 << (id & 31));
    }
}

APPEND_TEXT void EnsureCustomImpactsOwned(void)
{
    u16 i;

    if (!gRuntimeConfig.custom_gun_data)
        return;

    for (i = 0; i < gCustomImpactCount; i++)
    {
        u8 index = gCustomImpacts[i].index;
        u8 id = gCustomImpacts[i].id;

        gImpactOwned |= ((u32)1 << index);
        gGunDataBits[id >> 5] |= ((u32)1 << (id & 31));
    }

    /* Cannon / bullet ids are outside the 49..76 impact sync loop, so unlock
     * them on the same status-screen entry paths that unlock impacts. */
    EnsureCustomCannonsOwned();
    EnsureCustomBulletsOwned();
}

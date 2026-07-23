#include "global.h"
#include "runtime.h"
#include "ram_map.h"

#define MAX_BOMBS 7

#define PLAYER_STATE_OFF 8
#define PLAYER_STATE_HURT 6
#define PLAYER_STATE_FLY 7

#define CANNON_ID_LO 1
#define CANNON_ID_HI 28
#define BULLET_ID_LO 29
#define BULLET_ID_HI 48
#define IMPACT_ID_LO 49
#define IMPACT_ID_HI 76

/* Item bits in gItemsOwned (see DATA CARD / FLARE / FOSSIL string table). */
#define KEY_ITEMS_MASK ((u32)0xFFF40001)      /* bit0 DATA CARD; 18+ GENOME/VIRUS/… */
#define OVERWORLD_ITEMS_MASK ((u32)0x0013FFFE) /* bits 1–17 flares+fossils; bit 20 hint */

/* Vanilla UpdateShooterFrame @ 0x08014E70: bl 0x14A48; bl 0x1749C */
typedef void (*VoidFunc)(void);

static void SetGunDataBits(int idLo, int idHi)
{
    int id;

    for (id = idLo; id <= idHi; id++)
        gGunDataBits[id >> 5] |= ((u32)1 << (id & 31));
}

/* Unlock flags are persistent — apply once. Re-writing gun-data bits every
 * frame makes CountGunData() always succeed and trips per-entity "owned"
 * branches that re-init encounter / HUD state. */
static void ApplyInventoryCheatsOnce(void)
{
    static u8 sApplied;

    if (sApplied)
        return;
    sApplied = 1;

    if (gRuntimeConfig.all_cannon_data)
    {
        gCannonOwned |= 0x0FFFFFFFu; /* 28 pieces */
        SetGunDataBits(CANNON_ID_LO, CANNON_ID_HI);
    }

    if (gRuntimeConfig.all_bullet_data)
    {
        gBulletOwned |= 0x000FFFFFu; /* 20 pieces */
        SetGunDataBits(BULLET_ID_LO, BULLET_ID_HI);
    }

    if (gRuntimeConfig.all_impact_data)
    {
        gImpactOwned |= 0x0FFFFFFFu; /* 28 pieces */
        SetGunDataBits(IMPACT_ID_LO, IMPACT_ID_HI);
    }

    if (gRuntimeConfig.all_key_items)
        gItemsOwned |= KEY_ITEMS_MASK;

    if (gRuntimeConfig.all_overworld_items)
        gItemsOwned |= OVERWORLD_ITEMS_MASK;
}

/* 0x03007080 is the flight HUD quota / enemy-remaining digit (vanilla writes
 * 6, 13, 30, …) — not ship HP. Infinite health cancels the hurt state instead. */
static void ApplyMaxHealth(void)
{
    u8 *player = gPlayerPtr;

    if (player != NULL && player[PLAYER_STATE_OFF] == PLAYER_STATE_HURT)
        player[PLAYER_STATE_OFF] = PLAYER_STATE_FLY;
}

APPEND_TEXT void UpdateShooterFrame__Replacement(void)
{
    /* Vanilla meter / kill credit first, then light refills. */
    ((VoidFunc)0x08014A49)();
    ((VoidFunc)0x0801749D)();

    if (gRuntimeConfig.always_max_health)
        ApplyMaxHealth();

    if (gRuntimeConfig.always_max_bombs && gPlayerBombs < MAX_BOMBS)
        gPlayerBombs = MAX_BOMBS;

    ApplyInventoryCheatsOnce();
}

#include "global.h"
#include "runtime.h"

#define PLAYER_PTR ((u8 **)0x03000DB8)
#define PLAYER_HP ((u8 *)0x03007080)
#define PLAYER_BOMBS ((u8 *)0x03007014)
#define PLAYER_IFRAME_OFF 22
#define MAX_HP 6
#define MAX_BOMBS 7
#define IFRAME_FRAMES 120

/* Owned Gun Data bit array (IDs 1..76). */
#define GUN_DATA_BITS ((u32 *)0x03007690)
/* Per-type ownership words (bits 0..N-1). */
#define CANNON_OWNED ((u32 *)0x03007744)
#define BULLET_OWNED ((u32 *)0x0300773C)
#define IMPACT_OWNED ((u32 *)0x03007748)
/* Category 3: key / overworld items. */
#define ITEMS_OWNED ((u32 *)0x03007740)

#define CANNON_ID_LO 1
#define CANNON_ID_HI 28
#define BULLET_ID_LO 29
#define BULLET_ID_HI 48
#define IMPACT_ID_LO 49
#define IMPACT_ID_HI 76

/* Item bits in 0x03007740 (see DATA CARD / FLARE / FOSSIL string table). */
#define KEY_ITEMS_MASK ((u32)0xFFF40001)      /* bit0 DATA CARD; 18+ GENOME/VIRUS/… */
#define OVERWORLD_ITEMS_MASK ((u32)0x0013FFFE) /* bits 1–17 flares+fossils; bit 20 hint */

/* Vanilla UpdateShooterFrame @ 0x08014E70: bl 0x14A48; bl 0x1749C */
typedef void (*VoidFunc)(void);

static void SetGunDataBits(int idLo, int idHi)
{
    int id;
    u32 *bits = GUN_DATA_BITS;

    for (id = idLo; id <= idHi; id++)
        bits[id >> 5] |= ((u32)1 << (id & 31));
}

static void ApplyInventoryCheats(void)
{
    if (gRuntimeConfig.all_cannon_data)
    {
        *CANNON_OWNED |= 0x0FFFFFFFu; /* 28 pieces */
        SetGunDataBits(CANNON_ID_LO, CANNON_ID_HI);
    }

    if (gRuntimeConfig.all_bullet_data)
    {
        *BULLET_OWNED |= 0x000FFFFFu; /* 20 pieces */
        SetGunDataBits(BULLET_ID_LO, BULLET_ID_HI);
    }

    if (gRuntimeConfig.all_impact_data)
    {
        *IMPACT_OWNED |= 0x0FFFFFFFu; /* 28 pieces */
        SetGunDataBits(IMPACT_ID_LO, IMPACT_ID_HI);
    }

    if (gRuntimeConfig.all_key_items)
        *ITEMS_OWNED |= KEY_ITEMS_MASK;

    if (gRuntimeConfig.all_overworld_items)
        *ITEMS_OWNED |= OVERWORLD_ITEMS_MASK;
}

APPEND_TEXT void UpdateShooterFrame__Replacement(void)
{
    u8 *player;

    if (gRuntimeConfig.always_max_health)
    {
        *PLAYER_HP = MAX_HP;
        player = *PLAYER_PTR;
        if (player != 0)
            *(u16 *)(player + PLAYER_IFRAME_OFF) = IFRAME_FRAMES;
    }

    if (gRuntimeConfig.always_max_bombs)
        *PLAYER_BOMBS = MAX_BOMBS;

    ApplyInventoryCheats();

    ((VoidFunc)0x08014A49)();
    ((VoidFunc)0x0801749D)();
}

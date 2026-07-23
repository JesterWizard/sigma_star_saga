#include "global.h"
#include "runtime.h"

#define PLAYER_PTR ((u8 **)0x03000DB8)
#define PLAYER_HP ((u8 *)0x03007080)
#define PLAYER_BOMBS ((u8 *)0x03007014)
#define PLAYER_IFRAME_OFF 22
#define MAX_HP 6
#define MAX_BOMBS 7
#define IFRAME_FRAMES 120

/* Vanilla UpdateShooterFrame @ 0x08014E70: bl 0x14A48; bl 0x1749C */
typedef void (*VoidFunc)(void);

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

    ((VoidFunc)0x08014A49)();
    ((VoidFunc)0x0801749D)();
}

#include "global.h"
#include "actor.h"
#include "ram_map.h"

/*
 * InitActorParams — peeled from baserom @ 0x080318B4 (0x48 bytes).
 * Vanilla-only; custom max-HP snapshot lives in src_custom via LynJump.
 */

void InitActorParams(u8 index, s32 hp, s32 param38, s32 expPool, u8 kind)
{
    u8 *actor = &gActorPool[(u32)index * ACTOR_STRIDE];

    *(s32 *)(actor + ACTOR_OFF_HP) = hp;
    *(s32 *)(actor + ACTOR_OFF_PARAM38) = param38;
    *(s32 *)(actor + ACTOR_OFF_EXP) = expPool;

    if (kind != 0xFF)
        actor[ACTOR_OFF_KIND] = kind;
    else
        actor[ACTOR_OFF_KIND] = 0x41;
}

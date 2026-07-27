#include "global.h"
#include "actor.h"
#include "actor_death.h"
#include "ram_map.h"

/*
 * ActorDeathAward — decompiled from baserom @ 0x080319B0 (span 0x52).
 * Vanilla-only; overworld EXP awards live in src_custom via LynJump.
 */

typedef void (*DeleteActorFunc)(u32 index);
typedef void (*VoidFunc)(void);
typedef void (*PlaySfxFunc)(u32 id, u32 pan);
typedef void (*GemCreateFunc)(u32 gemType, s32 x, s32 y, u32 kind, s32 expPool);

#define DeleteActorRom ((DeleteActorFunc)0x08006311)
#define DeathAwardFx ((VoidFunc)0x0802F35D)
#define PlaySfx ((PlaySfxFunc)0x080003C5)
#define GemCreate ((GemCreateFunc)0x0803B7D1)

#define GEM_TYPE_NONE 0xFF
#define GEM_CREATE_KIND 0x21
#define DEATH_SFX_ID 0x62

void ActorDeathAward(u8 index, u32 gemType, s32 expPool)
{
    u8 *actor;
    s32 x;
    s32 y;

    DeleteActorRom(index);

    if (expPool != 0)
        DeathAwardFx();

    if (gemType == GEM_TYPE_NONE)
        return;

    PlaySfx(DEATH_SFX_ID, 3);

    actor = &gActorPool[(u32)index * ACTOR_STRIDE];
    /* Integer halves of 16.16 world positions (vanilla ldrsh @ +2). */
    x = *(s16 *)(actor + ACTOR_OFF_X + 2);
    y = *(s16 *)(actor + ACTOR_OFF_Y + 2);
    GemCreate(gemType, x, y, GEM_CREATE_KIND, expPool);
}

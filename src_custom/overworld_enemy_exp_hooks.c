#include "global.h"
#include "runtime.h"
#include "ram_map.h"
#include "actor.h"
#include "damage.h"
#include "level_up.h"
#include "data_structures.h"
#include "overworld_enemy_ids.h"

/*
 * Overworld fauna kill EXP — C only (no ASM continues).
 *
 * Primary: LynJump DamageApply @ 0x08005350.
 * Fallback: PlayerDeathFx → TryAwardOverworldFaunaKillExp.
 *
 * HUD: sync gPlayerExpDisplay and HudUpdate(1) so NEXT digits refresh
 * (overworld never runs flight HudSync alone in a useful way after we
 * equalize display==exp).
 */

#define ACTOR_OFF_TYPE  0x20
#define ACTOR_OFF_MODEL 0x22

#define OW_EXP_AWARDED_MARK 0xE2E2E2E2u

/* Default when anim is known fauna but catalog has no row. */
#define OVERWORLD_FAUNA_DEFAULT_EXP 15

APPEND_TEXT static int IsFaunaAnim(u16 anim)
{
    switch (anim)
    {
    case OVERWORLD_ANIM_BURROWING_WORM:
    case OVERWORLD_ANIM_JUMPING_SPIDER:
    case OVERWORLD_ANIM_SLUG:
    case OVERWORLD_ANIM_HENNK:
    case OVERWORLD_ANIM_SHELL_BEETLE:
    case OVERWORLD_ANIM_TICK_SPIDER:
    case OVERWORLD_ANIM_JUMPING_SPIDER_ALT:
    case OVERWORLD_ANIM_CEPHALOPOD:
    case OVERWORLD_ANIM_MECH_SPIDER:
    case OVERWORLD_ANIM_HANGING_SPIDER:
    case OVERWORLD_ANIM_MULTI_EYE:
    case OVERWORLD_ANIM_ZOMBIE:
        return 1;
    default:
        return 0;
    }
}

APPEND_TEXT static void AwardOverworldFaunaExp(u8 *actor)
{
    u16 type;
    u16 anim;
    u32 amount;

    if (actor == NULL)
        return;
    if (*(u32 *)(actor + ACTOR_OFF_EXP) == OW_EXP_AWARDED_MARK)
        return;

    type = *(u16 *)(actor + ACTOR_OFF_TYPE);
    anim = *(u16 *)(actor + ACTOR_OFF_MODEL);
    if (anim == 0)
        return;

    amount = LookupOverworldEnemyExp(type, anim);
    if (amount == 0 && IsFaunaAnim(anim))
        amount = OVERWORLD_FAUNA_DEFAULT_EXP;
    if (amount == 0)
        return;

    *(u32 *)(actor + ACTOR_OFF_EXP) = OW_EXP_AWARDED_MARK;
    AddExperience(amount);

    gPlayerExpDisplay = gPlayerExp;
    ((void (*)(u32))0x08010E59)(1);
}

APPEND_TEXT void TryAwardOverworldFaunaKillExp(void)
{
    u8 *actor;
    u8 index;

    if (!gRuntimeConfig.overworld_enemy_exp)
        return;

    actor = gPlayerPtr;
    if (actor == NULL)
    {
        index = gCurrentActorIndex;
        if (index >= ACTOR_COUNT)
            return;
        actor = &gActorPool[(u32)index * ACTOR_STRIDE];
    }

    AwardOverworldFaunaExp(actor);
}

APPEND_TEXT bool8 DamageApply__Replacement(u8 index, s32 damage, u16 flags)
{
    u8 *actor;
    s32 hp_before;
    bool8 ok;

    if (index >= ACTOR_COUNT)
        return DamageApply(index, damage, flags);

    actor = &gActorPool[(u32)index * ACTOR_STRIDE];
    hp_before = *(s32 *)(actor + ACTOR_OFF_HP);

    ok = DamageApply(index, damage, flags);

    if (gRuntimeConfig.overworld_enemy_exp
        && ok
        && index != 0
        && hp_before > 0
        && *(s32 *)(actor + ACTOR_OFF_HP) <= 0)
    {
        AwardOverworldFaunaExp(actor);
    }

    return ok;
}

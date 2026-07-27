#include "global.h"
#include "actor.h"
#include "damage.h"
#include "ram_map.h"

/*
 * DamageApply — decompiled from baserom @ 0x08005350 (span 0x88).
 * Vanilla-only; overworld kill EXP lives in src_custom via LynJump.
 *
 * Linked into append (.text). Callers at 0x08005350 are redirected to
 * DamageApply__Replacement when .overworld_enemy_exp is on; that hook
 * calls this append copy so the veneer is not re-entered.
 */

#define ACTOR_FLAG_INVULN 0x1800 /* bits checked before applying damage */
#define DAMAGE_FLAG_FORCE 0x8000 /* skip invuln check when set */
#define DAMAGE_FLAG_MASK  0x7FFF

bool8 DamageApply(u8 index, s32 damage, u16 flags)
{
    u8 *actor;
    u16 apply_flags;
    s32 hp;
    s32 dealt;

    apply_flags = flags & DAMAGE_FLAG_MASK;

    if ((flags & DAMAGE_FLAG_FORCE) == 0)
    {
        actor = &gActorPool[(u32)index * ACTOR_STRIDE];
        if ((*(u16 *)(actor + ACTOR_OFF_FLAGS) & ACTOR_FLAG_INVULN) != 0)
            return FALSE;
    }

    actor = &gActorPool[(u32)index * ACTOR_STRIDE];
    hp = *(s32 *)(actor + ACTOR_OFF_HP);

    if (hp < damage)
        dealt = hp;
    else
        dealt = damage;

    *(s32 *)(actor + ACTOR_OFF_PARAM38) = dealt;
    *(s32 *)(actor + ACTOR_OFF_HP) = hp - dealt;
    *(u16 *)(actor + ACTOR_OFF_FLAGS) |= apply_flags;
    return TRUE;
}

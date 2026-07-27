#ifndef GUARD_OVERWORLD_ENEMY_IDS_H
#define GUARD_OVERWORLD_ENEMY_IDS_H

/*
 * Overworld / field fauna identifiers.
 *
 * OVERWORLD_TYPE_*  — SpawnActor type (r2), stored at actor+0x20
 * OVERWORLD_ANIM_*  — SpawnActor model (r1), stored at actor+0x22 (AI dispatch key)
 *
 * Keep in sync with src_custom/data_structures/overworld_enemy_exp.json
 * (tools/compile_data_structures.py resolves JSON names from this header).
 */

/* SpawnActor type ids (actor +0x20). */
enum {
    OVERWORLD_TYPE_SLUG = 0,
    OVERWORLD_TYPE_BURROWING_WORM = 20,
    OVERWORLD_TYPE_MECH_SPIDER = 37,
    OVERWORLD_TYPE_MULTI_EYE = 40,
    OVERWORLD_TYPE_SHELL_BEETLE = 41,
    OVERWORLD_TYPE_HANGING_SPIDER = 43,
    OVERWORLD_TYPE_JUMPING_SPIDER_ALT = 46,
    OVERWORLD_TYPE_HENNK = 49,
    OVERWORLD_TYPE_CEPHALOPOD = 50,
    OVERWORLD_TYPE_TICK_SPIDER = 52,
    OVERWORLD_TYPE_ZOMBIE = 53,
    OVERWORLD_TYPE_JUMPING_SPIDER = 64,
};

/* AI / ANM model ids (actor +0x22). */
enum {
    OVERWORLD_ANIM_BURROWING_WORM = 13,
    OVERWORLD_ANIM_JUMPING_SPIDER = 36,
    OVERWORLD_ANIM_SLUG = 51,
    OVERWORLD_ANIM_HENNK = 61,
    OVERWORLD_ANIM_SHELL_BEETLE = 62,
    OVERWORLD_ANIM_TICK_SPIDER = 63,
    OVERWORLD_ANIM_JUMPING_SPIDER_ALT = 64,
    OVERWORLD_ANIM_CEPHALOPOD = 65,
    OVERWORLD_ANIM_MECH_SPIDER = 66,
    OVERWORLD_ANIM_HANGING_SPIDER = 67,
    OVERWORLD_ANIM_MULTI_EYE = 69,
    OVERWORLD_ANIM_ZOMBIE = 70,
};

#endif /* GUARD_OVERWORLD_ENEMY_IDS_H */

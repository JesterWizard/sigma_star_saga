#include "runtime.h"

const RuntimeConfig gRuntimeConfigRom __attribute__((section(".runtime_config"))) = {
    /* SELECT+L during 2D flight → win (see asm/flight_skip.s). */
    .skip_flight_battle = TRUE,
    .always_run = TRUE,
    .always_max_health = FALSE,
    .always_max_bombs = TRUE,
    .all_cannon_data = TRUE,
    .all_bullet_data = TRUE,
    .all_impact_data = TRUE,
    .all_key_items = TRUE,
    .all_tools = TRUE,
    .exp_multiplier = 1,
    .custom_enemy_exp = TRUE,
    .custom_dialogue = TRUE,
    .custom_gun_data = TRUE,
    .enemy_hp_bars = TRUE,
    .disable_random_battles = FALSE,
};

#include "runtime.h"

const RuntimeConfig gRuntimeConfigRom __attribute__((section(".runtime_config"))) = {
    /* SELECT+L during 2D flight → win (see asm/flight_skip.s). */
    .skip_flight_battle = TRUE,
    .always_run = TRUE,
    .always_max_health = FALSE,
    .always_max_bombs = FALSE,
    .all_cannon_data = TRUE,
    .all_bullet_data = TRUE,
    .all_impact_data = TRUE,
    .all_key_items = FALSE,
    .all_tools = FALSE,
    .exp_multiplier = 3,
    .custom_enemy_exp = FALSE,
    .overworld_enemy_exp = TRUE,
    .custom_dialogue = FALSE,
    .custom_gun_data = TRUE,
    .enemy_hp_bars = FALSE,
    .disable_random_battles = TRUE,
    .custom_cutscene_ch1 = TRUE,
    .custom_cutscene_stage = TRUE,
    .custom_talk_helpers = FALSE,
    .custom_event_runner = FALSE, /* interpret compile_events opcode tables */
};

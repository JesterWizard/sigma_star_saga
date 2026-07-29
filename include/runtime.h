#ifndef GUARD_RUNTIME_CONFIG_H
#define GUARD_RUNTIME_CONFIG_H

#include "gba/types.h"

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

/*
 * Build-time hack toggles (ROM const copy).
 * Edit configs/runtime.c, then rebuild.
 */
typedef struct {
    u8 skip_flight_battle; /* SELECT+L in a 2D flight stage → stage clear */
    u8 always_run;         /* Overworld run speed without holding B */
    u8 always_max_health;  /* Flight: cancel hurt state (do not touch enemy counter) */
    u8 always_max_bombs;   /* Flight: smart bombs held at maximum (7) */
    /* 3 Gun Data types (Cannon / Bullet / Impact) */
    u8 all_cannon_data;    /* Unlock every Cannon Data piece */
    u8 all_bullet_data;    /* Unlock every Bullet Data piece */
    u8 all_impact_data;    /* Unlock every Impact Data piece */
    u8 all_key_items;      /* Full gItemsOwned (CB Have All Items) */
    u8 all_tools;          /* Krill Puck / Boots / Girl Wings / Scanner / Warp */
    u8 exp_multiplier;     /* Scale AddExperience amount (1 = vanilla) */
    u8 level_cap_255;      /* Allow level-ups through 255 (vanilla max 99) */
    u8 start_max_level;    /* Force gPlayerLevel to cap (99, or 255 if lifted) */
    u8 custom_enemy_exp;   /* Build enemy_exp.json by-id catalog (not amount remap) */
    u8 overworld_enemy_exp; /* Award EXP on overworld fauna kills (JSON catalog) */
    u8 custom_dialogue;    /* Use src_custom/dialogue banks (append + ptr redirect) */
    u8 custom_gun_data; /* Custom Impact Data from impact_data.json */
    u8 enemy_hp_bars;   /* Flight: HP bars under enemies (soft OAM only) */
    u8 disable_random_battles; /* Skip overworld RNG encounters (0x1DA5C) */
    u8 custom_cutscene_ch1; /* LynJump Ch.1 opener FSM → CutsceneCh1Opener peel */
    u8 custom_cutscene_stage; /* LynJump stage FSM @ 0x2B18C → CutsceneStageUpdate */
    u8 custom_talk_helpers; /* LynJump StartTalk* / InitTalkBanks peels */
    u8 custom_event_runner; /* Drive known cutscenes from compile_events tables */
    u8 custom_gax_audio; /* GAX_SPEECH + custom music/voice catalogs / event ops */
} RuntimeConfig;

extern const RuntimeConfig gRuntimeConfigRom;
#define gRuntimeConfig gRuntimeConfigRom

#define _maybe_unused __attribute__((unused))

#define LYN_REPLACE_CHECK(name) \
    extern __typeof__(name) name; \
    static const void *const _maybe_unused lyn_exists_check_##name = &name

#define APPEND_TEXT __attribute__((section(".append_text")))
#define APPEND_RODATA __attribute__((section(".append_rodata")))
#define APPEND_DATA __attribute__((section(".append_data")))

#endif /* GUARD_RUNTIME_CONFIG_H */

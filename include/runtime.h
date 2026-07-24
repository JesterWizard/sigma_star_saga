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
    u8 all_key_items;      /* DATA CARD, GENOME, VIRUS SAMPLE, … */
    u8 all_overworld_items; /* Flares, fossils, hints, … */
    u8 exp_multiplier;     /* Scale AddExperience amount (1 = vanilla) */
    u8 custom_dialogue;    /* Use src_custom/dialogue banks (append + ptr redirect) */
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

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
    u8 skip_flight_battle; /* Select / L / Start in a 2D flight stage → stage clear */
    u8 always_run;         /* Overworld run speed without holding B */
    u8 always_max_health;  /* Flight: full HP + invincibility frames every tick */
    u8 always_max_bombs;   /* Flight: smart bombs held at maximum (7) */
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

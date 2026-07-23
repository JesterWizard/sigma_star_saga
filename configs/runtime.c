#include "runtime.h"

const RuntimeConfig gRuntimeConfigRom __attribute__((section(".runtime_config"))) = {
    .skip_flight_battle = TRUE,
    .always_run = TRUE,
    .always_max_health = TRUE,
    .always_max_bombs = TRUE,
};

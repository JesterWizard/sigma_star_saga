#ifndef GUARD_OVERWORLD_FRAME_H
#define GUARD_OVERWORLD_FRAME_H

#include "gba/types.h"

/*
 * Overworld / field main frame (baserom @ 0x0800D610).
 *
 * Shared JT entry for several modes. Relevant field modes: 4–9 and 15–23.
 *
 * Frame ownership (vanilla order):
 *   D610  body start
 *     1A1A0  world/map prep
 *     28A6C  actor/world step
 *     14E70  UpdateShooterFrame (no-op on field)
 *     18C7C  field helpers
 *     115C4  more field work
 *     …counters / HUD…
 *     A35C / 4B5C / 5940 / 90EC / 42E0 / 45B4 / 46E0 / 9898
 *     1114C(0)
 *   D66A  frame tail
 *     10E58(0)  soft-input / HUD sync (when enabled)
 *     8F50(0)   UpdateCameras — also DrawActors when arg == 0
 *     …epilogue…
 *
 * Soft display mirrors are submitted on VBlank (@ 0x080039B8), not here.
 * Input (gKeysHeld) is refreshed by the main loop before the mode JT.
 */

void OverworldMainFrame(void);
void UpdateCameras(u32 drawActors);

/* Soft camera scroll mirrors written by UpdateCameras (not HW BG*OFS). */
extern s32 gCamScrollMirrorX; /* 0x03000CF8 */
extern s32 gCamScrollMirrorY; /* 0x03001E70 */

#endif /* GUARD_OVERWORLD_FRAME_H */

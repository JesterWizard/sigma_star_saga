#include "global.h"
#include "overworld_frame.h"
#include "ram_map.h"
#include "actor.h"

/*
 * OverworldMainFrame / UpdateCameras — readable reference (baserom).
 *
 * Linked into append (not peeled). The debug-menu LynJump sites call the
 * baserom copies via OverworldMainFrame__Continue / absolute Thumb addresses.
 *
 * UpdateCameras @ 0x08008F50:
 *   - Writes soft scroll mirrors gCamScrollMirrorX/Y from camera 0
 *   - Walks camera layers, programs soft BG scroll
 *   - When arg == 0, also runs DrawActors
 */

#define VanillaUpdateCameras ((void (*)(u32))0x08008F51)

void UpdateCameras(u32 drawActors)
{
    VanillaUpdateCameras(drawActors);
}

void OverworldMainFrame(void)
{
    /*
     * Baserom @ 0x0800D610 — see include/overworld_frame.h for the call list.
     * Not invoked from C; the main-loop JT enters it with mov pc (no LR).
     */
    (void)0;
}

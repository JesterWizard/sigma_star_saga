#ifndef GUARD_EVENT_MACROS_H
#define GUARD_EVENT_MACROS_H

#include "dialogue_macros.h"

/*
 * Authoring macros for cutscene / overworld event scripts.
 * Parsed later by an event compiler (ygodm8-style); currently stubs.
 *
 * Layout mirrors https://github.com/JesterWizard/ygodm8 events/scripts/:
 *   EVENT_SCRIPT_REPLACEMENT(vanilla_addr, name[, on_false[, on_true]])
 *     …commands…
 *   END_EVENT_SCRIPT()
 *
 * Talk lines use the same TALK/TEXT/… macros as dialogue banks.
 * tools/compile_dialogue.py extracts TALK/TEXT from EVENT_SCRIPT bodies.
 *
 * Command names below match behavior peeled from CutsceneCh1Opener
 * (@ 0x080523EC) and related helpers — provisional until more FSMs land.
 */

#define EVENT_NOP 0

#define EVENT_SCRIPT_REPLACEMENT(vanilla_addr, name, ...)
#define EVENT_SCRIPT(name, ...)
#define END_EVENT_SCRIPT()

/* --- Timing / waits (scroll & counters are 16.16 fixed unless noted) ------ */

/* Spin until IWRAM counter @ 0x03003688 > n (Ch.1 step 0). */
#define WAIT_COUNTER_GT(...)

/* Wait while world scroll Y is <= / > threshold. */
#define WAIT_SCROLL_Y_LE(...)
#define WAIT_SCROLL_Y_GT(...)

/* Wait while world scroll X is <= / > threshold. */
#define WAIT_SCROLL_X_LE(...)
#define WAIT_SCROLL_X_GT(...)

/* Wait while cutscene velocity param[i] is <= / > threshold. */
#define WAIT_VEL_LE(...)
#define WAIT_VEL_GT(...)

/* Approximate frame delay (ygodm8 DELAY) — not yet mapped to a SSS helper. */
#define DELAY(...)

/* --- Cutscene state ------------------------------------------------------ */

/* gCutsceneParam[0]=dx, [1]=dy (16.16 scroll velocity). */
#define SET_SCROLL_VEL(dx, dy)
#define SET_SCROLL_VEL_X(...)
#define SET_SCROLL_VEL_Y(...)

/* Add to gCutsceneParam[0] / [1]. */
#define ADD_SCROLL_VEL_X(...)
#define ADD_SCROLL_VEL_Y(...)

/* Clamp gCutsceneParam component to an absolute value. */
#define CLAMP_SCROLL_VEL_X(...)
#define CLAMP_SCROLL_VEL_Y(...)

/* Advance FSM step (documentary; runtime still uses gCutsceneStep). */
#define SET_CUTSCENE_STEP(...)

/* --- Camera / world / actors --------------------------------------------- */

/* Add delta to gWorldScrollY and camera X (@ +0x2C). */
#define SCROLL_WORLD_Y(...)

/* Add delta to gWorldScrollY, camera X, and camera +0x34. */
#define SCROLL_WORLD_Y_CAM2(...)

/* Move every active gActorPool slot on X (2D flight cutscene pool shove). */
#define MOVE_FLYING_ACTORS_X(...)

/* Same, plus Y bump for actor indices 10..19 (Ch.1 step 12). */
#define MOVE_FLYING_ACTORS_XY(...)

/* --- Actor show / hide / spawn / walk (Ch.1 pool vocabulary) ------------- */

/* Cardinal dirs for WALK_ACTOR_DIR (matches WalkActorDir). */
#define ACTOR_DIR_RIGHT 0
#define ACTOR_DIR_DOWN  1
#define ACTOR_DIR_LEFT  2
#define ACTOR_DIR_UP    3

/* Soft visibility at (x, y) — u16 world units (same as SPAWN_ACTOR). */
#define SHOW_ACTOR(index, x, y)
#define HIDE_ACTOR(index)

/* Full despawn — DeleteActor @ 0x08006310 (frees gfx, clears flags). */
#define DELETE_ACTOR(index)

/* SpawnActor @ 0x08006178 — (camera, anim, type, x, y); flags default ACTIVE. */
#define SPAWN_ACTOR(camera, anim, type, x, y)

/* Absolute place at (x, y). */
#define MOVE_ACTOR(index, x, y)

/* Walk toward (x, y) at speed (16.16); sets VX/VY. */
#define WALK_ACTOR(index, x, y, speed)

/* Cardinal-only velocity (optional; prefer WALK_ACTOR with coords). */
#define WALK_ACTOR_DIR(index, dir, speed)

/* Per-slot velocity (actor+0x48 / +0x54, 16.16). */
#define SET_ACTOR_VEL_X(index, vx)
#define SET_ACTOR_VEL_Y(index, vy)

/* --- Actor / FX helpers (ROM stubs until named) -------------------------- */

/* Helper_2CD40 + Helper_318A0 pair (enable/disable actor control). */
#define SET_ACTOR_CTRL(enable, unused)

/* Helper_52340 — per-frame cutscene tick A. */
#define CUTSCENE_TICK_A()

/* Helper_52370 — per-frame cutscene tick B. */
#define CUTSCENE_TICK_B()

/* Helper_3B850 — one-shot FX after scroll-in. */
#define CUTSCENE_FX()

/* Helper_09070 — screen fade / transition. */
#define FADE_SCREEN(...)

/* gUnk_03000BF4 &= mask (palette / FX bits). */
#define PALETTE_AND(...)

/* Random gate used in Ch.1 step 2 (Helper_07348 & 0x1F). */
#define WAIT_RAND_CLEAR(...)

/* Input while step 15: adjust vel Y from KEY_UP / KEY_DOWN. */
#define SCROLL_VEL_FROM_DPAD()

/* Shared trailer: apply gCutsceneParam into camera slots. */
#define APPLY_CAMERA_DELTA()

#define FALLTHROUGH()
#define END()

#endif /* GUARD_EVENT_MACROS_H */

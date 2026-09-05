#include "event_macros.h"

/*
 * Chapter 1 opening cutscene + talk.
 *
 * Cutscene FSM: CutsceneCh1Opener @ 0x080523EC (LynJump when
 * .custom_cutscene_ch1 is TRUE). Talk bank entry: ROM 0x0805C9E1 / script id 51.
 *
 * Layout follows ygodm8 event scripts (commands + TALK in one body):
 *   https://github.com/JesterWizard/ygodm8/blob/797a52b56c8e1cd47f2851b8d532ead643f3936a/events/scripts/map_09_state_01.c
 *
 * Actor motion in vanilla is pool-wide MOVE_FLYING deltas (not absolute
 * walk waypoints). Flight setup SpawnActor sites @ 0x08053Bxx use ROM
 * immediates (mostly x=y=272; one at 374,368) or IWRAM table ldrsh.
 * Do not invent per-talk SPAWN/WALK coords around the StartTalk beat.
 *
 * Event macros are authoring / documentary until an event compiler exists;
 * compile_dialogue.py still packs TALK/TEXT into the talk bank.
 */

EVENT_SCRIPT_REPLACEMENT(0x0805C9E1, scene_05C9E1)

  /* === step 0 — enable control, wait counter, kick scroll vel ========= */
  SET_ACTOR_CTRL(1, 0)
  WAIT_COUNTER_GT(0x2C)
  SET_SCROLL_VEL(0x40000, 0)
  FADE_SCREEN(0, 2, 1, 0)
  SET_CUTSCENE_STEP(1)

  /* === step 1 — hold, tick A, wait scroll Y, disable + FX ============== */
  SET_ACTOR_CTRL(1, 0)
  CUTSCENE_TICK_A()
  WAIT_SCROLL_Y_GT(250 << 18)
  SET_ACTOR_CTRL(0, 0)
  CUTSCENE_FX()
  SET_CUTSCENE_STEP(2)

  /* === step 2 — approach Y; damp vel X or random fade ================== */
  CUTSCENE_TICK_A()
  WAIT_SCROLL_Y_GT(0x22C40000)
  ADD_SCROLL_VEL_X(0xFFFFFD80)
  WAIT_VEL_LE(0, 0x7FFF)
  CLAMP_SCROLL_VEL_X(0x8000)
  SET_CUTSCENE_STEP(3)
  /* else branch while Y still low: */
  WAIT_RAND_CLEAR(0x1F)
  FADE_SCREEN(0, 2, 1, 0)

  /* === step 3 — tick B, ramp vel, then dive vel Y ====================== */
  CUTSCENE_TICK_B()
  WAIT_SCROLL_Y_GT(0x2C600000)
  ADD_SCROLL_VEL_X(0x800)
  CLAMP_SCROLL_VEL_X(0x40000)
  WAIT_SCROLL_Y_GT(0x2EE00000)
  ADD_SCROLL_VEL_Y(0xFFFFFE00)
  WAIT_VEL_LE(1, 0xFFFF0000)
  CLAMP_SCROLL_VEL_Y(0xFFFF0000)
  SET_CUTSCENE_STEP(6)

  /* === step 6 — horizontal pull: world Y + move flying actors X ======== */
  CUTSCENE_TICK_B()
  WAIT_SCROLL_X_LE(0x0E5DFFFF)
  SCROLL_WORLD_Y(0xFBC00000)
  MOVE_FLYING_ACTORS_X(0x40000 - (0x88 << 19))
  SET_SCROLL_VEL_X(0x8000)
  SET_CUTSCENE_STEP(5)

  /* === step 5 — settle vel Y, then talk (script id 51) ================= */
  CUTSCENE_TICK_B()
  WAIT_SCROLL_X_LE(0x0D92FFFF)
  ADD_SCROLL_VEL_Y(0x200)
  WAIT_VEL_GT(1, 0)
  CLAMP_SCROLL_VEL_Y(0)
  SET_CUTSCENE_STEP(10)
  /* Vanilla: StartTalkById(51, 4, 3) */
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "A Krill Battleworm is dropping out of orbit. Sigma Team, follow my lead.")
  /* Vanilla also had:
   * TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_NEUTRAL,
   *     "Haha! We've wiped out a mile of 'em. Have we beaten the captain yet?")
   * TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
   *     "Focus! A Krill Battleworm is dropping out of orbit. Sigma Team, follow my lead.")
   * TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_NEUTRAL, "Yes sir!")
   */

  /* === step 10 — push world/actors on X ================================ */
  CUTSCENE_TICK_B()
  WAIT_SCROLL_Y_GT(0x34260000)
  SCROLL_WORLD_Y(0x0BEE0000)
  MOVE_FLYING_ACTORS_X(0x0BEE0000)
  SET_CUTSCENE_STEP(11)

  /* === step 11 — tick A+B, settle vel, clear palette bits ============== */
  CUTSCENE_TICK_B()
  CUTSCENE_TICK_A()
  WAIT_SCROLL_Y_GT(0x43C60000)
  ADD_SCROLL_VEL_Y(0x200)
  WAIT_VEL_GT(1, 0x8000)
  CLAMP_SCROLL_VEL_Y(0x8000)
  PALETTE_AND(0xF9FF)
  SET_CUTSCENE_STEP(12)

  /* === step 12 — big scroll + actor X/Y bump for slots 10..19 ========== */
  CUTSCENE_TICK_B()
  WAIT_SCROLL_X_GT(0x0E380000)
  SCROLL_WORLD_Y_CAM2(0xF0000000)
  MOVE_FLYING_ACTORS_XY(0xFFF80000 - (0x80 << 21), 0x80000)
  SET_SCROLL_VEL_X(0x80000)
  SET_CUTSCENE_STEP(13)

  /* === step 13 — damp vel Y to 0 ======================================= */
  CUTSCENE_TICK_B()
  WAIT_SCROLL_X_GT(0x0F3C0000)
  ADD_SCROLL_VEL_Y(0xFFFFFE00)
  WAIT_VEL_LE(1, 0)
  CLAMP_SCROLL_VEL_Y(0)
  SET_CUTSCENE_STEP(14)

  /* === step 14 — ease vel X down while tick A ========================== */
  ADD_SCROLL_VEL_X(0xFFFFF800) /* while vel X > 0x8000 */
  CUTSCENE_TICK_A()

  /* === step 15 — D-pad adjusts vel Y =================================== */
  SCROLL_VEL_FROM_DPAD()
  CUTSCENE_TICK_B()

  /* steps 4,7,8,9 — empty fallthrough to camera apply */
  APPLY_CAMERA_DELTA()
  FALLTHROUGH()

END_EVENT_SCRIPT()

#include "event_macros.h"

/*
 * Script id 109 — Target Counter briefing.
 *
 * Owner: stage FSM @ 0x0802B18C, outer JT case 0 @ 0x0802BFA0
 * (CutsceneCh1TargetBrief in src/cutscene_ch1.c). Talk ROM 0x080605BB.
 *
 * Vanilla TargetBrief only waits then StartTalkById — no SPAWN/WALK.
 * Event macros are authoring / documentary; runtime still runs vanilla Thumb
 * until the parent stage is LynJumped. compile_dialogue.py packs TALK only.
 */

EVENT_SCRIPT_REPLACEMENT(0x080605BB, scene_0605BB)

  /* === sub-step 0 → 1 ================================================== */
  SET_CUTSCENE_STEP(1)

  /* === sub-step 1 — wait counter, then talk ============================ */
  WAIT_COUNTER_GT(0xB4)
  SET_CUTSCENE_STEP(2)
  /* Vanilla: StartTalkById(109, 4, 3) */

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_NEUTRAL,
      "OK human! Listen up!",
      "See that number up there?",
      "That's your Target Counter!",
      "When it reaches zero...",
      "The threat is over.",
      "Take 'em down!"
  )

  /* Shared case trailer: camera += gCutsceneParam >> 1 */
  APPLY_CAMERA_DELTA()
  END()

END_EVENT_SCRIPT()

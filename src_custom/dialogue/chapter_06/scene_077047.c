#include "event_macros.h"

/*
 * Script id 451 — cutscene-fired talk.
 *
 * StageCase175 in CutsceneStageUpdate @ 0x0802B18C; StartTalkById @ 0x0802B648.
 * Talk ROM 0x08077047.
 *
 * No invented SPAWN/WALK — only macros evidenced by the owner FSM.
 * compile_dialogue.py packs TALK/TEXT; compile_events.py may emit a runner table.
 */

EVENT_SCRIPT_REPLACEMENT(0x08077047, scene_077047)

  /* === StageCase175 — wait counter, then talk ===================== */
  WAIT_COUNTER_GT(0xB3) /* vanilla fires when counter == 0xB4 */
  /* Vanilla: StartTalkById(451, 4, 3) */

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "We've got to find the heart of the monster!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What's your plan?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "We'll weaken it...",
      "Create an opening into its bloodstream!",
      "And insert the virus there!"
  )
  END()

END_EVENT_SCRIPT()

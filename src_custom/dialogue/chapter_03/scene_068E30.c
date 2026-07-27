#include "event_macros.h"

/*
 * Script id 226 — cutscene-fired talk.
 *
 * FSM StartTalkById @ 0x0804D9F0.
 * Talk ROM 0x08068E30.
 *
 * No invented SPAWN/WALK — only macros evidenced by the owner FSM.
 * compile_dialogue.py packs TALK/TEXT; compile_events.py may emit a runner table.
 */

EVENT_SCRIPT_REPLACEMENT(0x08068E30, scene_068E30)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Good. 5 more to go.")
  END()

END_EVENT_SCRIPT()

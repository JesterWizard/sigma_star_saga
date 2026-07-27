#include "event_macros.h"

/*
 * Script id 375 — cutscene-fired talk.
 *
 * FSM @ 0x08049454; StartTalkById @ 0x08049552.
 * Talk ROM 0x08071E72.
 *
 * No invented SPAWN/WALK — only macros evidenced by the owner FSM.
 * compile_dialogue.py packs TALK/TEXT; compile_events.py may emit a runner table.
 */

EVENT_SCRIPT_REPLACEMENT(0x08071E72, scene_071E72)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Ok, I've made visual contact with Scarlet.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Pick her up already!")
  END()

END_EVENT_SCRIPT()

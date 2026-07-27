#include "event_macros.h"

/*
 * Script id 455 — cutscene-fired talk.
 *
 * Map interact path; StartTalkById @ 0x0800D3FA.
 * Talk ROM 0x0807724F.
 *
 * No invented SPAWN/WALK — only macros evidenced by the owner FSM.
 * compile_dialogue.py packs TALK/TEXT; compile_events.py may emit a runner table.
 */

EVENT_SCRIPT_REPLACEMENT(0x0807724F, scene_07724F)

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Its loaded and ready!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "FIRE!!")
  END()

END_EVENT_SCRIPT()

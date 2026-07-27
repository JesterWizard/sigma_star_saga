#include "event_macros.h"

/*
 * Script id 453 — cutscene-fired talk.
 *
 * FSM StartTalkById @ 0x0804D37C.
 * Talk ROM 0x08077146.
 *
 * No invented SPAWN/WALK — only macros evidenced by the owner FSM.
 * compile_dialogue.py packs TALK/TEXT; compile_events.py may emit a runner table.
 */

EVENT_SCRIPT_REPLACEMENT(0x08077146, scene_077146)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "We did it! It's now or never!")
  END()

END_EVENT_SCRIPT()

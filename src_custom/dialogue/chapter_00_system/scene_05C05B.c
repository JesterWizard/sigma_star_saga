#include "event_macros.h"

/*
 * Script id 9 — cutscene-fired talk.
 *
 * JT FSM @ 0x08025394; StartTalkById @ 0x080257C8.
 * Talk ROM 0x0805C05B.
 *
 * No invented SPAWN/WALK — only macros evidenced by the owner FSM.
 * compile_dialogue.py packs TALK/TEXT; compile_events.py may emit a runner table.
 */

EVENT_SCRIPT_REPLACEMENT(0x0805C05B, scene_05C05B)

  TEXT("Save was cancelled.")
  END()

END_EVENT_SCRIPT()

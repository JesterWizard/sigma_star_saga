#include "event_macros.h"

/*
 * Script id 8 — cutscene-fired talk.
 *
 * JT FSM @ 0x08025394; StartTalkById @ 0x08025514 / 0x08025772.
 * Talk ROM 0x0805C035.
 *
 * No invented SPAWN/WALK — only macros evidenced by the owner FSM.
 * compile_dialogue.py packs TALK/TEXT; compile_events.py may emit a runner table.
 */

EVENT_SCRIPT_REPLACEMENT(0x0805C035, scene_05C035)

  TEXT("Save over existing data?\n  Yes\n  No")
  CHOICE()
  END()

END_EVENT_SCRIPT()

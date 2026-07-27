#include "event_macros.h"

/*
 * Script id 150 — cutscene-fired talk.
 *
 * FSM StartTalkById @ 0x0804EA64.
 * Talk ROM 0x080632E9.
 *
 * No invented SPAWN/WALK — only macros evidenced by the owner FSM.
 * compile_dialogue.py packs TALK/TEXT; compile_events.py may emit a runner table.
 */

EVENT_SCRIPT_REPLACEMENT(0x080632E9, scene_0632E9)

  TEXT("You got the COMM ROOM KEY!")
  END()

END_EVENT_SCRIPT()

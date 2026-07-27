#include "event_macros.h"

/*
 * Script id 0 — cutscene-fired talk.
 *
 * Beat chain ~0x0801BE74; StartTalkById @ 0x0801CA0A.
 * Talk ROM 0x0805BF74.
 *
 * No invented SPAWN/WALK — only macros evidenced by the owner FSM.
 * compile_dialogue.py packs TALK/TEXT; compile_events.py may emit a runner table.
 */

EVENT_SCRIPT_REPLACEMENT(0x0805BF74, scene_05BF74)

  TEXT("You found new GUN DATA!")
  END()

END_EVENT_SCRIPT()

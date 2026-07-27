#include "event_macros.h"

/*
 * Script id 6 — cutscene-fired talk.
 *
 * Beat chain ~0x0801BE74; StartTalkById @ 0x0801C18A.
 * Talk ROM 0x0805C005.
 *
 * No invented SPAWN/WALK — only macros evidenced by the owner FSM.
 * compile_dialogue.py packs TALK/TEXT; compile_events.py may emit a runner table.
 */

EVENT_SCRIPT_REPLACEMENT(0x0805C005, scene_05C005)

  TEXT("You found the VIRUS SAMPLE!")
  END()

END_EVENT_SCRIPT()

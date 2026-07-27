#include "event_macros.h"

/* Script id 38 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C6AF. */
EVENT_SCRIPT_REPLACEMENT(0x0805C6AF, scene_05C6AF)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "My parasite is named Zarg. He can't talk or anything, but that's what I call him.")
  END()

END_EVENT_SCRIPT()

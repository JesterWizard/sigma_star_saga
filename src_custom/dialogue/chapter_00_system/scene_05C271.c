#include "event_macros.h"

/* Script id 19 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C271. */
EVENT_SCRIPT_REPLACEMENT(0x0805C271, scene_05C271)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Do you like the music in here? It's good, but we only get one station.")
  END()

END_EVENT_SCRIPT()

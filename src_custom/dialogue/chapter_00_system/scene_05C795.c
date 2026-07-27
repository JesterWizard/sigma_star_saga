#include "event_macros.h"

/* Script id 41 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C795. */
EVENT_SCRIPT_REPLACEMENT(0x0805C795, scene_05C795)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Bloss is my uncle. Just so you know.")
  END()

END_EVENT_SCRIPT()

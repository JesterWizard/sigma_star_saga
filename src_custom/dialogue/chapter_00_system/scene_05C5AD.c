#include "event_macros.h"

/* Script id 33 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C5AD. */
EVENT_SCRIPT_REPLACEMENT(0x0805C5AD, scene_05C5AD)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "What time is it?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Sorry, I don't have a watch.")
  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_NEUTRAL,
      "What's a watch?")
  END()

END_EVENT_SCRIPT()

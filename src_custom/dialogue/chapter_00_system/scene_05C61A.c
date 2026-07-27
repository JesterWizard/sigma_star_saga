#include "event_macros.h"

/* Script id 35 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C61A. */
EVENT_SCRIPT_REPLACEMENT(0x0805C61A, scene_05C61A)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Hey, sorry about what we did to your planet.")
  END()

END_EVENT_SCRIPT()

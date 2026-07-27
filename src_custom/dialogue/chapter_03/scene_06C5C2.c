#include "event_macros.h"

/* Script id 294 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806C5C2. */
EVENT_SCRIPT_REPLACEMENT(0x0806C5C2, scene_06C5C2)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Launch all fighters! Every available pilot to launch bay NOW!!")
  END()

END_EVENT_SCRIPT()

#include "event_macros.h"

/* Script id 121 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080616E3. */
EVENT_SCRIPT_REPLACEMENT(0x080616E3, scene_0616E3)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Through here's the COMM ROOM.",
      "We use it to make long range calls to anywhere in the galaxy."
  )
  END()

END_EVENT_SCRIPT()

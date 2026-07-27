#include "event_macros.h"

/* Script id 310 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806D82A. */
EVENT_SCRIPT_REPLACEMENT(0x0806D82A, scene_06D82A)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "We're extracting samples of alien matter from the planet below. We've got samples from the Forest, Fire, and Ice planets on the way soon. Bet we discover something amazing!")
  END()

END_EVENT_SCRIPT()

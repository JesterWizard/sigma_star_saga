#include "event_macros.h"

/* Script id 122 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08061744. */
EVENT_SCRIPT_REPLACEMENT(0x08061744, scene_061744)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "The Tyrannical Overlord of Krill High Command has been setting up starbases like this one all over the quadrant. I wonder what he's looking for?")
  END()

END_EVENT_SCRIPT()

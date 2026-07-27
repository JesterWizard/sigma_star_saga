#include "event_macros.h"

/* Script id 140 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08062739. */
EVENT_SCRIPT_REPLACEMENT(0x08062739, scene_062739)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "Better get back to work, Earthman!")
  END()

END_EVENT_SCRIPT()

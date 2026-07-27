#include "event_macros.h"

/* Script id 123 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080617DB. */
EVENT_SCRIPT_REPLACEMENT(0x080617DB, scene_0617DB)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "You're the Earthman? Well, whatever. Just another name to try and remember.")
  END()

END_EVENT_SCRIPT()

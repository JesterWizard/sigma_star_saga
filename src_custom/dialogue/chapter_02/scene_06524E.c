#include "event_macros.h"

/* Script id 171 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806524E. */
EVENT_SCRIPT_REPLACEMENT(0x0806524E, scene_06524E)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "Heard about your battle in the fissure. How big was this creature again?")
  END()

END_EVENT_SCRIPT()

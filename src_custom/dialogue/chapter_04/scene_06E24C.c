#include "event_macros.h"

/* Script id 329 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E24C. */
EVENT_SCRIPT_REPLACEMENT(0x0806E24C, scene_06E24C)

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Here it comes...",
      "Coolness! We found the GENOME!",
      "Let's get it back to the Lab."
  )
  END()

END_EVENT_SCRIPT()

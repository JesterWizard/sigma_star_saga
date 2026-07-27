#include "event_macros.h"

/* Script id 309 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806D7E9. */
EVENT_SCRIPT_REPLACEMENT(0x0806D7E9, scene_06D7E9)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Have you seen the human female?! What a shiny coat it has!")
  END()

END_EVENT_SCRIPT()

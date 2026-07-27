#include "event_macros.h"

/* Script id 386 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08072523. */
EVENT_SCRIPT_REPLACEMENT(0x08072523, scene_072523)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "I'm ready to follow your orders sir!")
  END()

END_EVENT_SCRIPT()

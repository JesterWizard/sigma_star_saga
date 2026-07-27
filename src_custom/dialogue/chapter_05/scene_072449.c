#include "event_macros.h"

/* Script id 383 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08072449. */
EVENT_SCRIPT_REPLACEMENT(0x08072449, scene_072449)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Greetings Commander. Starbase 5 is at your complete disposal sir!")
  END()

END_EVENT_SCRIPT()

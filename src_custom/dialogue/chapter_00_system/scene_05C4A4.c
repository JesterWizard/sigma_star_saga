#include "event_macros.h"

/* Script id 29 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C4A4. */
EVENT_SCRIPT_REPLACEMENT(0x0805C4A4, scene_05C4A4)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "You're lucky. The fastest thing I've ever flown is a carrier.")
  END()

END_EVENT_SCRIPT()

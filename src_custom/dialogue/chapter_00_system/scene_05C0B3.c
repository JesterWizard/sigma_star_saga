#include "event_macros.h"

/* Script id 12 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C0B3. */
EVENT_SCRIPT_REPLACEMENT(0x0805C0B3, scene_05C0B3)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "It's an honor to serve with you, sir.")
  END()

END_EVENT_SCRIPT()

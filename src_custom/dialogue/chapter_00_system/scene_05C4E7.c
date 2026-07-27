#include "event_macros.h"

/* Script id 30 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C4E7. */
EVENT_SCRIPT_REPLACEMENT(0x0805C4E7, scene_05C4E7)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Did you know Krill ships have over 15,000 types of attack? Really, try them out.")
  END()

END_EVENT_SCRIPT()

#include "event_macros.h"

/* Script id 66 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805CFE5. */
EVENT_SCRIPT_REPLACEMENT(0x0805CFE5, scene_05CFE5)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "Just my luck...the one ship in the armada with remote clearance to this room gets blowed up. Guess I'm not getting in here again.")
  END()

END_EVENT_SCRIPT()

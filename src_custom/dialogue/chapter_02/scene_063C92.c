#include "event_macros.h"

/* Script id 157 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08063C92. */
EVENT_SCRIPT_REPLACEMENT(0x08063C92, scene_063C92)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Go set up those beacons. Remember, you're looking for three patches of soft soil.")
  END()

END_EVENT_SCRIPT()

#include "event_macros.h"

/* Script id 60 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805CDB9. */
EVENT_SCRIPT_REPLACEMENT(0x0805CDB9, scene_05CDB9)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "Hey pilot. You can check your status on the Status Screen. Just press SELECT.")
  END()

END_EVENT_SCRIPT()

#include "event_macros.h"

/* Script id 57 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805CCA9. */
EVENT_SCRIPT_REPLACEMENT(0x0805CCA9, scene_05CCA9)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "Yeeeeehaw!")
  END()

END_EVENT_SCRIPT()

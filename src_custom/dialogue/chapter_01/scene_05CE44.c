#include "event_macros.h"

/* Script id 62 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805CE44. */
EVENT_SCRIPT_REPLACEMENT(0x0805CE44, scene_05CE44)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "Hey pilot. Life sure is unpredictable. The actions we take in life...they shape our future. Remember that.")
  END()

END_EVENT_SCRIPT()

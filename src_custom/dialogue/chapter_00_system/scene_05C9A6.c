#include "event_macros.h"

/* Script id 50 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C9A6. */
EVENT_SCRIPT_REPLACEMENT(0x0805C9A6, scene_05C9A6)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "I can't wait to get back to my wife and kid again.")
  END()

END_EVENT_SCRIPT()

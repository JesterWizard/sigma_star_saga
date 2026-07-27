#include "event_macros.h"

/* Script id 313 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806DCA6. */
EVENT_SCRIPT_REPLACEMENT(0x0806DCA6, scene_06DCA6)

  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "No time for smalltalk Earthman.")
  END()

END_EVENT_SCRIPT()

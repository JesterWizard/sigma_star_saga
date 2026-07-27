#include "event_macros.h"

/* Script id 273 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806AFBB. */
EVENT_SCRIPT_REPLACEMENT(0x0806AFBB, scene_06AFBB)

  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Is it done?")
  END()

END_EVENT_SCRIPT()

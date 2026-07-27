#include "event_macros.h"

/* Script id 371 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08071AAB. */
EVENT_SCRIPT_REPLACEMENT(0x08071AAB, scene_071AAB)

  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Looks like you have an incoming transmission in the Comm Room.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'll take it here.")
  END()

END_EVENT_SCRIPT()

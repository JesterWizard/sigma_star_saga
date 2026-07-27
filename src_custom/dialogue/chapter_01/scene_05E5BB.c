#include "event_macros.h"

/* Script id 80 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805E5BB. */
EVENT_SCRIPT_REPLACEMENT(0x0805E5BB, scene_05E5BB)

  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Get up, Earth dog!")
  END()

END_EVENT_SCRIPT()

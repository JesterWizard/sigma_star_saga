#include "event_macros.h"

/* Script id 164 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080642AB. */
EVENT_SCRIPT_REPLACEMENT(0x080642AB, scene_0642AB)

  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Find the fissure. It will probably be in an unstable area.")
  END()

END_EVENT_SCRIPT()

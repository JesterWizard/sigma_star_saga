#include "event_macros.h"

/* Script id 240 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08069247. */
EVENT_SCRIPT_REPLACEMENT(0x08069247, scene_069247)

  TALK(SPEAKER_EK, SIDE_RIGHT, EXPR_NEUTRAL,
      "There you are, finally! Commander Zelly said your arrival would be unusual. Let's see, we've got Recker and Pysme, and...")
  END()

END_EVENT_SCRIPT()

#include "event_macros.h"

/* Script id 134 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080623FC. */
EVENT_SCRIPT_REPLACEMENT(0x080623FC, scene_0623FC)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Sigh...",
      "That arrogant...",
      "I'd better get to Psyme before I miss the boat."
  )
  END()

END_EVENT_SCRIPT()

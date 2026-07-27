#include "event_macros.h"

/* Script id 326 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E0D6. */
EVENT_SCRIPT_REPLACEMENT(0x0806E0D6, scene_06E0D6)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I got the LAST FOSSIL! I'd better find Scarlet!")
  END()

END_EVENT_SCRIPT()

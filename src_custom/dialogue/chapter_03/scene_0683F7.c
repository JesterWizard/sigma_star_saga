#include "event_macros.h"

/* Script id 214 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080683F7. */
EVENT_SCRIPT_REPLACEMENT(0x080683F7, scene_0683F7)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Um...She's not serious! Really!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What's with the bloodbath?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I found Scarlet-")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Scarlet?")
  END()

END_EVENT_SCRIPT()

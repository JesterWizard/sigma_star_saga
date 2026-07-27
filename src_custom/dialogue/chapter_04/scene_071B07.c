#include "event_macros.h"

/* Script id 372 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08071B07. */
EVENT_SCRIPT_REPLACEMENT(0x08071B07, scene_071B07)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "This is Recker!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I found our stray sheep.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Where is she?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'll tell you in person. Get to Starbase 4 on the double.")
  END()

END_EVENT_SCRIPT()

#include "event_macros.h"

/* Script id 395 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08072E9A. */
EVENT_SCRIPT_REPLACEMENT(0x08072E9A, scene_072E9A)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "There you are! Is this place the Nexus you were talking about?")
  TALK(SPEAKER_SISTERS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Yes. Very loud!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I don't hear anything. ")
  TALK(SPEAKER_SISTERS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Loud! Too loud!! Hurting!")
  END()

END_EVENT_SCRIPT()

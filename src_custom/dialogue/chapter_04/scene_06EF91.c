#include "event_macros.h"

/* Script id 346 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806EF91. */
EVENT_SCRIPT_REPLACEMENT(0x0806EF91, scene_06EF91)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme! There you are. How come you ran off like that?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Later. Something's not right here.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'll check it.")
  END()

END_EVENT_SCRIPT()

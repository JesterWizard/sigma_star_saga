#include "event_macros.h"

/* Script id 195 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806764C. */
EVENT_SCRIPT_REPLACEMENT(0x0806764C, scene_06764C)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Hmm...Huh?",
      "What's going on?"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Didn't you feel that? We've landed.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Oh, yeah. Sure is quiet.")
  END()

END_EVENT_SCRIPT()

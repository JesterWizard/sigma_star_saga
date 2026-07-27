#include "event_macros.h"

/* Script id 416 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08074CE0. */
EVENT_SCRIPT_REPLACEMENT(0x08074CE0, scene_074CE0)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "...Groan...")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Remember that little chip I put behind your ear?",
      "For all our little one-way chit chats?",
      "At this range it also gives me direct control of your central nervous system.",
      "It's time to say goodbye now."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "YAHRRGHHHH!!!")
  END()

END_EVENT_SCRIPT()

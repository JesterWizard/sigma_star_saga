#include "event_macros.h"

/* Script id 96 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805FA8A. */
EVENT_SCRIPT_REPLACEMENT(0x0805FA8A, scene_05FA8A)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Thanks, I guess.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "But then, my parasite is a primitive animal.",
      "How about you?  What did they fit you with?"
  )
  END()

END_EVENT_SCRIPT()

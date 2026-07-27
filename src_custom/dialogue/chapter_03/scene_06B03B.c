#include "event_macros.h"

/* Script id 275 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806B03B. */
EVENT_SCRIPT_REPLACEMENT(0x0806B03B, scene_06B03B)

  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Greetings Recker. I trust all of your loose ends are tied up?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yeah. I should be done with Zelly for good.",
      "Where is Scarlet?"
  )
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "The Earth girl? She continues to misbehave.",
      "Ek is breaking her in.",
      "She'll acknowledge her master in time! Ha!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Let me see her.")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "It will have to wait. Take these.")
  END()

END_EVENT_SCRIPT()

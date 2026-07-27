#include "event_macros.h"

/* Script id 331 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E383. */
EVENT_SCRIPT_REPLACEMENT(0x0806E383, scene_06E383)

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "What happened to her?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I don't know!")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "Uh oh. I know what this is.",
      "How to explain this to off-worlders...",
      "Mister Recker, how well do you know Psyme?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Meaning?!")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "When a Krill girl... well... Take a closer look.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What is that growing on her back?")
  END()

END_EVENT_SCRIPT()

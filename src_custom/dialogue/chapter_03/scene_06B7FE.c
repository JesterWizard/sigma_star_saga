#include "event_macros.h"

/* Script id 287 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806B7FE. */
EVENT_SCRIPT_REPLACEMENT(0x0806B7FE, scene_06B7FE)

  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker, you've been a good soldier. But this obsession with the Earth girl is getting you into trouble that you don't need.",
      "Psyme has learned this lesson the hard way."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You did this to her because I asked her to watch Keys?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I...I deserved it. I was insubordinate.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "So this is how you run your starbase? Treating people like animals?")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "That's all you are, Earthman! Don't forget your place in my presence!")
  END()

END_EVENT_SCRIPT()

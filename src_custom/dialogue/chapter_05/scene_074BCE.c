#include "event_macros.h"

/* Script id 415 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08074BCE. */
EVENT_SCRIPT_REPLACEMENT(0x08074BCE, scene_074BCE)

  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "You've gotten a lot smarter Recker.",
      "That electric charge was originally meant for you once your mission was completed."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I was an idiot to believe anything you said.",
      "The devil was in the details after all."
  )
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Yes he was. And here is one last detail you overlooked.")
  END()

END_EVENT_SCRIPT()

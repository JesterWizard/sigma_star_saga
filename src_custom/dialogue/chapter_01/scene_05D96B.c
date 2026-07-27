#include "event_macros.h"

/* Script id 74 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805D96B. */
EVENT_SCRIPT_REPLACEMENT(0x0805D96B, scene_05D96B)

  TALK(SPEAKER_GRUNT_1, SIDE_LEFT, EXPR_NEUTRAL,
      "See? See?! It moved again!",
      "It's breathing! That thing is alive and breathing!"
  )
  TALK(SPEAKER_GRUNT_2, SIDE_RIGHT, EXPR_NEUTRAL,
      "You're imagining things.")
  TALK(SPEAKER_GRUNT_1, SIDE_LEFT, EXPR_NEUTRAL,
      "I did no such thing! Watch this! I'll prove it to you!")
  TALK(SPEAKER_GRUNT_2, SIDE_RIGHT, EXPR_NEUTRAL,
      "Hands off you bloodthirsty fool!",
      "That'll kill it for sure!",
      "Let's just push it back into space. We'll say the crate was empty."
  )
  TALK(SPEAKER_GRUNT_1, SIDE_LEFT, EXPR_NEUTRAL,
      "Are you crazy? Stand between Commander Bloss and a new Earthman?! No way.")
  TALK(SPEAKER_GRUNT_2, SIDE_RIGHT, EXPR_NEUTRAL,
      "Fine. Remember you found the Earthman, I found the gun.")
  TALK(SPEAKER_GRUNT_1, SIDE_LEFT, EXPR_NEUTRAL,
      "Hold on...",
      "Nope. No good. The breathing stopped again.",
      "Guess...",
      "...things...",
      "...after all..."
  )
  END()

END_EVENT_SCRIPT()

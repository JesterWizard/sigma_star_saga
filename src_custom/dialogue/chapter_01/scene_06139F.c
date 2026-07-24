#include "dialogue_macros.h"

/* ROM 0x0806139F */
DIALOGUE_SCRIPT(0x0806139F, scene_06139F)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Well, mission accomplished, huh? I guess that's that.",
      "You know, I have to admit...",
      "I thought you'd embarrass me somehow, but you managed to stay in one piece."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "It's what I do.",
      "So what now?"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'll process the readings we got from the human mine and compare them to our own surface readings. The final data will go to Krill High Command for review.",
      "In the meantime, show that scanner to Commander Bloss...He'll want to see it."
  )
END_DIALOGUE_SCRIPT()

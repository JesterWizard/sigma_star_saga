#include "dialogue_macros.h"

/* ROM 0x0806C855 */
DIALOGUE_SCRIPT(0x0806C855, scene_06C855)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme! I can't do this!!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I know you still care about your people! But these pilots will kill all of us! You've got to defend this station!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I can't betray them!!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Look! See this ship?",
      "It's one of Sigma's, better than new! You said you'd get him for me!",
      "Did you mean what you said?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "...")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Sigma will be leading the attack, I just know it!",
      "Please, Recker!"
  )
END_DIALOGUE_SCRIPT()

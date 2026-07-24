#include "dialogue_macros.h"

/* ROM 0x0806DD84 */
DIALOGUE_SCRIPT(0x0806DD84, scene_06DD84)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "No!",
      "Yes!",
      "What I mean is, I've been ordered to earn the trust of the Krill around me. It's necessary."
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "So when this is over, you go your separate ways?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Absolutely.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Good. Because I hate to see a perfectly good human wasted on that Krill trash!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Heh. You think I'm perfectly good, do you?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Don't read into that!",
      "Anyway, we need to split up.",
      "I'll check this way. Remember, we're looking for fossils."
  )
END_DIALOGUE_SCRIPT()

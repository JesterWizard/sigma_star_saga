#include "dialogue_macros.h"

/* ROM 0x0805F167 */
DIALOGUE_SCRIPT(0x0805F167, scene_05F167)
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Ah, you're back, Earthman. I just got an urgent call from Krill High Command.",
      "It's time to cash in that favor you owe me."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'm your man.")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "We're dropping this station onto a heavily forested planet. A lot of hostile life forms in the area. If I set you up with one of our Krill Fighters, can you blaze a path for us?",
      "I'd rather not risk one of my own men."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Just call me Ensign Expendable.")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Then get yourself to the launch bay.")
END_DIALOGUE_SCRIPT()

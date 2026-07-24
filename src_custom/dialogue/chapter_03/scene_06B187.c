#include "dialogue_macros.h"

/* ROM 0x0806B187 */
DIALOGUE_SCRIPT(0x0806B187, scene_06B187)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What's this for?")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's a way to remove obstacles. With the radiation gone, Psyme was able to detect a black box on the Ice Planet.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I found it on long range surface scan.",
      "It should contain data on the virus they were breeding."
  )
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Having the black box could help us create an anti-virus in case the humans try to breed one again.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Why not get the information from Scarlet?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Would you give this Earth girl thing a rest?!",
      "She's a prisoner of the Krill now, OK? Let us deal with her."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I don't like what I'm seeing here. She's a scientist, not a soldier. She could be an asset. Instead you're treating her like an animal!")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "You test my patience Mister Recker!",
      "This is my station and I'll run things as I see fit!",
      "You have your orders, now get out of here!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'll bring your black box. But we may need Scarlet Keys to interpret the contents of the box. Think it over before you do something rash.")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Your concern is noted, Recker! Now clear out!")
END_DIALOGUE_SCRIPT()

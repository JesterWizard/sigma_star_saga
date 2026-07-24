#include "dialogue_macros.h"

/* ROM 0x0805F323 */
DIALOGUE_SCRIPT(0x0805F323, scene_05F323)
  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Just heard the news. You're flying a Krill Fighter?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Seems that way.")
  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_NEUTRAL,
      "Ok, well this ain't like no human crate. Krill Fighters are living beings, just like your armor. Don't spook it whatever you do.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "The ship's alive?")
  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_NEUTRAL,
      "It chitchats with your personal parasite, that's how you control it. You'll also be sharing your energy and experience with any ship you pilot.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "So if my ship gets the sniffles?")
  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_NEUTRAL,
      "Just take care of her and she'll take care of you. While you're in the Control Cavity, you and your ship share life and limb.",
      "So you can see why saving your progress is a good idea before flying."
  )
END_DIALOGUE_SCRIPT()

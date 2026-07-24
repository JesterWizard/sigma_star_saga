#include "dialogue_macros.h"

/* ROM 0x0806F007 */
DIALOGUE_SCRIPT(0x0806F007, scene_06F007)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "The virus! And anti-virus! They're gone!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "It must've been Scarlet! I never should have trusted her!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Hang on. It could have been Professor Zart.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "The anti-virus would have taken longer to incubate. So whoever the thief was would have needed to either wait until both were done, or steal them separately as they were completed.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Scarlet would have waited for the anti-virus.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "And Zart would have only needed the original virus, and the Genome. So there's a time span between suspects.",
      "I need to find out when the crew went missing.",
      "Search the control center for clues."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Ok.")
END_DIALOGUE_SCRIPT()

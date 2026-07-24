#include "dialogue_macros.h"

/* ROM 0x0806F2A0 */
DIALOGUE_SCRIPT(0x0806F2A0, scene_06F2A0)
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "This had better be good Recker.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "It's good all right. Someone's made off with the virus. That germ you were willing to sacrifice the entire Earth fleet for, it's gone without a trace.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "No, it's quite safe.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "How? You think Scarlet Keys has it?")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "I have it.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "That's impossible.")
END_DIALOGUE_SCRIPT()

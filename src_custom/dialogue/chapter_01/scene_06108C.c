#include "dialogue_macros.h"

/* ROM 0x0806108C */
DIALOGUE_SCRIPT(0x0806108C, scene_06108C)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Well Recker! How did it feel to trample your own species?",
      "Pretty liberating, eh!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Nothing about that felt good.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "While you were flirting with that clanking monstrosity, I did a little reconnaissance.",
      "I think your Earth people were digging for the same thing we were searching for ourselves.",
      "I was able to secure their Scanning Tool, as well as their readings. They're much more accurate than ours."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Leave it to us humans to take the least subtle approach.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'll take the readings, you hold onto this tool.")
END_DIALOGUE_SCRIPT()

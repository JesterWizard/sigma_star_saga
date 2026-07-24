#include "dialogue_macros.h"

/* ROM 0x0805FBDE */
DIALOGUE_SCRIPT(0x0805FBDE, scene_05FBDE)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "And sensitive too! I'll have to learn to play nice.",
      "It's Psyme in case you're wondering.",
      "Easy to remember. Rhymes with Rhyme."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "My name. Bloss said you're tagging along.",
      "Be at the landing hatch in five minutes.",
      "It's next to the launch bay."
  )
END_DIALOGUE_SCRIPT()

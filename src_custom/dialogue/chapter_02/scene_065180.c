#include "dialogue_macros.h"

/* ROM 0x08065180 */
DIALOGUE_SCRIPT(0x08065180, scene_065180)
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Psyme and I will  report to Commander Zelly. She will want a conference with High Command in the Lab.",
      "No offense Recker, but I think you should sit this one out."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Whatever. I'll be in my quarters.")
END_DIALOGUE_SCRIPT()

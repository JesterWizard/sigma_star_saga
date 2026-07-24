#include "dialogue_macros.h"

/* ROM 0x080775F2 */
DIALOGUE_SCRIPT(0x080775F2, scene_0775F2)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme!",
      "Psyme! Wake up!"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "...moan...",
      "..."
  )
END_DIALOGUE_SCRIPT()

#include "dialogue_macros.h"

/* ROM 0x080693D9 */
DIALOGUE_SCRIPT(0x080693D9, scene_0693D9)
  TALK(SPEAKER_EK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Whup! Sounds like the site blasting's begun. Good thing you didn't dawdle, eh?",
      "Miss Psyme, be a good girl and wash that pet before giving her to Nomak. ",
      "Get some of that Earth stink off her."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Idiots.")
END_DIALOGUE_SCRIPT()

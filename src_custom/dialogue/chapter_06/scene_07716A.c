#include "dialogue_macros.h"

/* ROM 0x0807716A */
DIALOGUE_SCRIPT(0x0807716A, scene_07716A)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What are you doing?",
      "Use the virus!"
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "I have to go now...",
      "I should have told you."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Told me what?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Earlier, when I ran away...",
      "I gave myself the virus. It was the only way I could think of to keep it safe.",
      "I'm sorry."
  )
END_DIALOGUE_SCRIPT()

#include "dialogue_macros.h"

/* ROM 0x080736C6 */
DIALOGUE_SCRIPT(0x080736C6, scene_0736C6)
  TALK(SPEAKER_IOT, SIDE_RIGHT, EXPR_NEUTRAL,
      "No crime goes unpunished man of Earth.",
      "One planet as of yet remains.",
      "No mercy will be given to those who destroy life. "
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "The Krill Homeworld!! ",
      "That's the sixth planet!",
      "Tell me I'm right!!"
  )
  TALK(SPEAKER_IOT, SIDE_RIGHT, EXPR_NEUTRAL,
      "Soon the Sleeping Flesh will awaken. In its footsteps will be calamity. ",
      "Then we will sleep. Our retribution will be complete."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You can't allow this! Sure they're Krill, but you'll be wiping out billions of innocent people!",
      "Tell me how to stop it!!"
  )
  TALK(SPEAKER_IOT, SIDE_RIGHT, EXPR_NEUTRAL,
      "It can not be stopped.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Your people ruined my world! At least give me a chance!!")
END_DIALOGUE_SCRIPT()

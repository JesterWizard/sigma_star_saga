#include "dialogue_macros.h"

/* ROM 0x08072069 */
DIALOGUE_SCRIPT(0x08072069, scene_072069)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Ok, now that we actually have the anti-virus we can keep Tierney on a short leash. We know Earth has a virus to destroy the weapon if it falls to the Krill.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "And if Earth gets the bio weapon, then you still have me.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "So for the time being, the play field is level.",
      "The only thing we DON'T know is why both Tierney and the Tyrannical Overlord need six planet's worth of alien matter to form the weapon.",
      "Why wouldn't one do the job?"
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "All of the matter must be linked together for a reason.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "We'll find out soon enough.",
      "Commander Zelly sends a greeting.",
      "Starbase 5 has taken position over the Forgotten Planet."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Let's head out.")
END_DIALOGUE_SCRIPT()

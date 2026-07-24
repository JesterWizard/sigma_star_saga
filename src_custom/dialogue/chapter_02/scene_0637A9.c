#include "dialogue_macros.h"

/* ROM 0x080637A9 */
DIALOGUE_SCRIPT(0x080637A9, scene_0637A9)
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "If everyone's here, let me explain our mission.",
      "Krill High Command requires detailed scans of this planet.",
      "But to do that, we need to be able to find a fissure that will allow for a deep reading."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "No problem. We handled something like this before.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "But you weren't suspended over a boiling sea of lava at the time.",
      "Naturally, the weakest points on the landmass are best for scanning, but don't allow for the weight of the starbase. So, we'll need to land further out and set up force fields to act as bridges to span the crevices on foot."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Sounds dangerous.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's worse than you think. The kind of force fields he's talking about require beacons to be set up right against the edge of the crevice.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "So you'll need to go down to the surface, and post the beacons first. That will tell the crewmen here on the base where to project the force fields.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What's the hold up? Let's go already.")
END_DIALOGUE_SCRIPT()

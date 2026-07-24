#include "dialogue_macros.h"

/* ROM 0x0805F663 */
DIALOGUE_SCRIPT(0x0805F663, scene_05F663)
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Not as easy as it looked, eh Earthman? ",
      "Why don't you hook up with the landing party. Give you a chance to use that firecracker you're always carrying around.",
      "You like to blow stuff up right?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Who's leading this party?")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Don't get ahead of yourself kid. I've already got a team lead, troublesome as she is.",
      "She's a hand-me-down from the front lines. Guess High Command thinks it's funny to drop their garbage on me."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Front lines?")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "A month back over Earth.",
      "We gave this kid the best Battleworm in the fleet and she still managed to get taken down by a single human fighter. Total incompetent."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "A girl?")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "She was stripped of rank and sent here as punishment.",
      "Ha! But a few months with Bloss should straighten her out.",
      "She's probably in the Skinning Room. Why not drop by and say hi?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "...",
      "So that's who shot down Sigma Team! A lousy Krill girl!"
  )
END_DIALOGUE_SCRIPT()

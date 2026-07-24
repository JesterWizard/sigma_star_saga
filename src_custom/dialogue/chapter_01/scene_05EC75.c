#include "dialogue_macros.h"

/* ROM 0x0805EC75 */
DIALOGUE_SCRIPT(0x0805EC75, scene_05EC75)
  TALK(SPEAKER_SILENCE, SIDE_RIGHT, EXPR_NEUTRAL,
      "...............",
      "................",
      "................",
      "................"
  )
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker! Alive and kicking I see!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "No thanks to you!!",
      "You tried to kill me!"
  )
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Control yourself Recker. Your voice will carry.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I don't see what difference it makes since I've got a bug on my head listening in on every word! I'm as good as dead already!")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "That parasite won't be reporting anything.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "But that's not what Bloss said.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "He's lying.",
      "After your last mission we found Krill pilots dead in the wreckage. We've done extensive studies of their parasites."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "And?")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's harmless biotechnology. Mindless reactive creatures...",
      "To the Krill, no different than a wristwatch.",
      "Now listen carefully. I've just now pulled up our intel on Bloss.",
      "He's the lowest ranking commander in the Krill service. There's a reason his starbase is positioned in the middle of nowhere.",
      "Give him an angle that offers career advancement and you'll have him eating out of your hand."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "When should I contact you again, sir?")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'll signal you.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "How?")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Like this...")
END_DIALOGUE_SCRIPT()

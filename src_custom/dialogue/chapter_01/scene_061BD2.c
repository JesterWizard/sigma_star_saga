#include "dialogue_macros.h"

/* ROM 0x08061BD2 */
DIALOGUE_SCRIPT(0x08061BD2, scene_061BD2)
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker! Come over here!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What, am I in trouble?")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Hardly!",
      "You wiped out a solid mile of human scum!",
      "Far as I'm concerned, you're good as a Krill."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Uh...",
      "Thank you sir!"
  )
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "So it disappoints me to see it end here.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "End? What are we doing?")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Relax kid. I'm trying to tell you that you've been promoted.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Promoted?")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Krill High Command reviewed the readings from the Earth Mine the moment we got them. Said it was exactly what they needed.",
      "Destroying the human infestation put you over the top.",
      "I'm finally in with the chief mucky mucks now thanks to you. They won't forget my name after today.",
      "And I'll make sure I remember yours."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "So where am I being promoted to?")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "For starters, hold onto that Scanning Tool, you'll need it.",
      "You're heading to Starbase 2. It's stationed in orbit above a Fire Planet.",
      "And find Psyme. I'm throwing her into the deal.",
      "She'll be someone else's problem for a change."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You mean mine.")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Watch your back.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yes sir.")
END_DIALOGUE_SCRIPT()

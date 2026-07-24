#include "dialogue_macros.h"

/* ROM 0x08077E03 */
DIALOGUE_SCRIPT(0x08077E03, scene_077E03)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "How did we escape from that...that thing?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "We'd smuggled out a copy of the virus.",
      "Recker flew into the creature and destroyed it from the inside, right where the Overlord was fused to it."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "He deserved worse! He sacrificed our planet for his pet monster!",
      "What would drive a man to such action!!",
      "The Overlord I remember from my youth would never have done such horrific things."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Power is a dangerous and corrupting force. He wouldn't be the first to fall victim to it. ")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "But now that your people are free of him, they can start over and rebuild.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "That's impossible! There's nothing left.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "We did it.",
      "Earth is as beautiful now as it ever was.",
      "Our way of life and our world are still worth defending...worth dying for.",
      "Even if the planet's a little broken in."
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "The Krill will need real leaders after today.",
      "People like you."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Well, maybe some day I'll go back and pick up the pieces.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You're staying?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Right now, I just want to get to know you.",
      "The REAL you."
  )
END_DIALOGUE_SCRIPT()

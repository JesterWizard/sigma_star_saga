#include "dialogue_macros.h"

/* ROM 0x08060647 */
DIALOGUE_SCRIPT(0x08060647, scene_060647)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "So? How was it?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "That was amazing!",
      "We don't have anything like that on Earth!",
      "My parasite...and that ship...It was like I was in its head!"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I wouldn't get too attached to that ship.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Why not?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Because, silly. You never know which ship will be summoning you.",
      "It could be a different one every time.",
      "Which reminds me. Everything you experience is remembered by your parasite.",
      "That means that the stronger you become, the stronger the ship you inhabit will be."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Weird. On Earth, our ships do all the work. We just pilot them.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Which is why you won't benefit any from that peashooter you carry.",
      "Don't forget that.",
      "If you're going to strengthen the bond between you and your parasite, it'll be in the stars. Build up your parasite's experience level, and your ships will fly tough!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Got it.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'm going to scout ahead. We need to gather data on the area.")
END_DIALOGUE_SCRIPT()

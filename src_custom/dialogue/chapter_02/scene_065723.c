#include "dialogue_macros.h"

/* ROM 0x08065723 */
DIALOGUE_SCRIPT(0x08065723, scene_065723)
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "...And at this point, our human pilot was locked in combat with the creature.")
  TALK(SPEAKER_OVERLORD_SHADOW, SIDE_RIGHT, EXPR_NEUTRAL,
      "Most unusual.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Tyrannical Overlord sir, I have never seen or heard of a living creature of such magnitude! Forgive me for asking sir, but is this the ultimate weapon you've referred to?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "What insolence!")
  TALK(SPEAKER_OVERLORD_SHADOW, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's quite all right Commander Zelly. No, this living thing you've found is of no consequence. It is nothing more than a parasite, living off the matter I wish to exploit.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "A parasite?",
      "Then the matter you're speaking of, it's..."
  )
  TALK(SPEAKER_OVERLORD_SHADOW, SIDE_RIGHT, EXPR_NEUTRAL,
      "Much larger, yes.",
      "Tell me more about these tremors."
  )
END_DIALOGUE_SCRIPT()

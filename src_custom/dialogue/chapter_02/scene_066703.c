#include "dialogue_macros.h"

/* ROM 0x08066703 */
DIALOGUE_SCRIPT(0x08066703, scene_066703)
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Commander Zelly informed me about your unusual transfer.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I don't trust her. This feels wrong to me.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's just a precaution I'm sure. But I still can't understand the importance of these missions. Recker, High Command said that thing we battled on the Fire Planet was a parasite.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "That's enough Blune.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "What kind of planet has a half-mile long parasite living on it? This is getting way out of hand!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's none of our business what High Command does. Don't forget that High Command is carrying out the will of the Tyrannical Overlord. This is his wisdom you're questioning so show some restraint!")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's getting too crazy, Psyme! I want out of this!",
      "Do me a favor Recker. When you get to that new starbase, look up a guy named Ek. Tell him I want a transfer."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'll see what I can do.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'll never underestimate an Earthman again. You've been a loyal friend.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Thanks. Watch your back.")
END_DIALOGUE_SCRIPT()

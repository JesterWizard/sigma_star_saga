#include "dialogue_macros.h"

/* ROM 0x08077413 */
DIALOGUE_SCRIPT(0x08077413, scene_077413)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "First Blune...",
      "Now Psyme..."
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "There was nothing you could have done.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I just can't believe that.",
      "How can I live, knowing my actions cost them their lives?"
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "One day at a time, just like everyone else.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "...")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Never forget them, but try to remember everyone you saved here on Earth, Recker!",
      "Against all odds you won! You're a hero!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Then why does it feel like I lost?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "You didn't lose me. Come on. I'll buy you dinner.")
END_DIALOGUE_SCRIPT()

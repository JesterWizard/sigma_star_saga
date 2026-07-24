#include "dialogue_macros.h"

/* ROM 0x0806F3F4 */
DIALOGUE_SCRIPT(0x0806F3F4, scene_06F3F4)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Wha-",
      "Blune!",
      "What on Earth are you...",
      "ARE you on Earth?"
  )
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "You know Recker, war affects the lives of many people.",
      "Some would go to great lengths to see it end."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You were working for Tierney all this time?")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "No, of course not.",
      "But after our last meeting I was able to trace your line back to Commander Tierney. I'm sorry, but you gave me an opportunity. I took advantage of it."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "And you truly believe that giving this virus to Earth will save your people? Did our good Commander Tierney tell you its true purpose?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Blune knows that it's not meant to eradicate the Krill people.",
      "Its sole purpose is to kill the alien matter."
  )
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker, you were right. That alien matter is a bio weapon.",
      "If the Krill find all six planets first, they will use it to destroy Earth, and many planets after it. I believe the Tyrannical Overlord and High Command are working against the people's interests.",
      "That is why I went to Earth."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "And I suppose Tierney promised to use the virus to kill the alien bio weapon.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Yes.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "If you infect the alien matter, what will happen to the six planets?")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "They will, unfortunately, become uninhabitable.")
END_DIALOGUE_SCRIPT()

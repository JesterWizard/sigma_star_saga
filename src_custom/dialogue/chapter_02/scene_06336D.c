#include "dialogue_macros.h"

/* ROM 0x0806336D */
DIALOGUE_SCRIPT(0x0806336D, scene_06336D)
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker, good.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What is it this time?")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Are you getting settled?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yes. I'm operating under a woman named Commander Zelly. She doesn't trust me one bit. Completely by the book.",
      "I've been teamed up with a female soldier named Psyme, and now some guy...Blune. He's the commander's right hand man. A real Earth hater."
  )
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "He'll have valuable information, so do whatever you can to get close to him. Also, I've anticipated a more thorough investigation into your past by Krill High Command.",
      "We've recently discovered a piggybacked signal on AEF's lines. It's very likely a Krill wiretap. We're dummying up some documents that support your story, and we'll be placing the files where the Krill can find them. If they investigate your past, they'll find a detailed account of exactly who you said you were."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "They are sending me down for another survey of this Fire Planet. Tell me now Commander, is there another Earth base down there?")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "No. We have no knowledge of this planet or why the Krill would want it. As soon has you have concrete data, send it over.",
      "Tierney out!"
  )
END_DIALOGUE_SCRIPT()

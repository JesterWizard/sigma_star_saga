#include "dialogue_macros.h"

/* ROM 0x080777C9 */
DIALOGUE_SCRIPT(0x080777C9, scene_0777C9)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "It was Scarlet...",
      "Her body contained a copy of the virus.",
      "She sacrificed herself to save us all from that creature."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "She carried the virus all along?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "No, I don't think so.",
      "She must have injected herself with it before Blune stole the sample from the Lab.",
      "It's the only explanation. ",
      "...",
      "I can't stand it! There must have been another way!"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker, some people...they hold to a higher authority.",
      "They realize there are more important things than just themselves.",
      "And when those people find themselves in dire circumstances, amazing things can happen.",
      "A seemingly insignificant life can save an entire world from chaos and ruin.",
      "It's wrong of you to dishonor her sacrifice.",
      "Accept what she did for you, and hold her memory in high esteem."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You're right...",
      "I just wish there was something I could have done differently."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Were you in love with her?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "...")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's ok. You don't have to answer. ")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "...",
      "You know, the first time I met you I didn't like you very much."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Yeah?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "But once I got to know you, well...",
      "That's when I really wanted to strangle you.",
      "Now though..."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "You find me irresistible.",
      "Hee hee...",
      "Come on, show me around this planet of yours.",
      "I want to see everything!"
  )
END_DIALOGUE_SCRIPT()

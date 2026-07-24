#include "dialogue_macros.h"

/* ROM 0x08067FE4 */
DIALOGUE_SCRIPT(0x08067FE4, scene_067FE4)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Were those...",
      "Human beings?"
  )
  TALK(SPEAKER_UNKNOWN, SIDE_RIGHT, EXPR_NEUTRAL,
      "Yeah...what's left of 'em.",
      "You're a man, right?",
      "Like, a human man?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Oh, right. I'd almost forgotten about the Krill armor.",
      "Regular human, same as you."
  )
  TALK(SPEAKER_UNKNOWN, SIDE_RIGHT, EXPR_NEUTRAL,
      "Err...Why are you dressed up like that? You look ridiculous!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'll explain later. Who are you?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Scarlet Keys. I work here.",
      "Or I did, up till the accident anyways.",
      "Are you with a rescue team? Or a decontamination squad?",
      "You're not going to stick me in a plastic bubble are you?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "No, no. Look, Miss Keys you said?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "You're cute. You can call me Scarlet.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Ok, Scarlet. I'm only posing as a Krill to get information out of them. It's important that you play along.",
      "We'd both be in serious trouble if my cover's blown."
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Double agent gig. Ok, sure.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "First, I need you to tell me what this compound is here for.")
END_DIALOGUE_SCRIPT()

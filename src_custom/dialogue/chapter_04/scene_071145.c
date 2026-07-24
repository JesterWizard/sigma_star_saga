#include "dialogue_macros.h"

/* ROM 0x08071145 */
DIALOGUE_SCRIPT(0x08071145, scene_071145)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Are you Commander Ammer?")
  TALK(SPEAKER_AMMER, SIDE_RIGHT, EXPR_NEUTRAL,
      "I don't see you in my appointment book.",
      "Who are you and what can I do for you?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I am Recker. Previously assigned here to Commander Zelly.")
  TALK(SPEAKER_AMMER, SIDE_RIGHT, EXPR_NEUTRAL,
      "You are human!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yes. Can you tell me where to find the Commander?")
  TALK(SPEAKER_AMMER, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'm afraid the only commander on this starbase is, and has always been me!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "But I served with her here. I spoke to her the other day in this very room!")
  TALK(SPEAKER_AMMER, SIDE_RIGHT, EXPR_NEUTRAL,
      "You must be mistaken. Many starbases look alike, and they are rotated from time to time. You wouldn't be the first to walk into the wrong classroom so to speak.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I...suppose so. How embarrassing for me!")
  TALK(SPEAKER_AMMER, SIDE_RIGHT, EXPR_NEUTRAL,
      "Pay it no mind. Well, I have things to attend to!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yes me too. Forgive the interruption.")
END_DIALOGUE_SCRIPT()

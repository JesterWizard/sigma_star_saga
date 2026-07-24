#include "dialogue_macros.h"

/* ROM 0x0805E8CA */
DIALOGUE_SCRIPT(0x0805E8CA, scene_05E8CA)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Is that the best you've got?")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Splendid!",
      "Yes, from the moment I saw you under lock and key I knew you'd prove valuable!",
      "Your human leaders would vomit if they knew we had our hands on one of their most hated outcasts!",
      "I'll make you an offer."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'm listening...")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "How would you like to pay back the fools who exiled you?",
      "I'll let you live, and in exchange you can perform a couple er...",
      "Odd jobs."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You mean betray my planet.")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Ha ha ha. Does that bother you?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Not really. Does this mean I'm free to go?")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Hah! Leave! Take the grand tour! I'll even let you keep your sidearm!",
      "But never forget, my very loyal creature is fused to your spine, and my men are watching.",
      "Prove you can be relied upon and you will go far in the Krill forces. "
  )
END_DIALOGUE_SCRIPT()

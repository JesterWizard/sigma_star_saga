#include "dialogue_macros.h"

/* ROM 0x08070BAF */
DIALOGUE_SCRIPT(0x08070BAF, scene_070BAF)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Ok, the last one is activated and seems to be linked to the other two. Now how do I read this thing?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "We're talking about a resonating pulse. It will be audio, so I can decode it from here.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Are you picking up anything?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Yes, but the signal is only strong enough to get a locale of one planet.",
      "But something's wrong.",
      "It's been removed from the star charts as a condemned world. A Forgotten Planet."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "That's weird. So no way to ID the last planet?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Unfortunately no.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Well, one is better than nothing.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "It may be worse than nothing. There is no starbase there.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "We'll need to pull a favor then.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "How?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Who do we have dirt on? Besides Blune?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Well, there's Commander Zelly on Starbase 2.",
      "But she wouldn't help. She hates us."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "She hates you.",
      "I'm her loyal servant who executed Blune, remember?",
      "Man, looking back I almost wish I had."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Don't say that. Blune may be a screw up, but at least he thinks he's doing what's right for the people.",
      "At least he's not a back stabber."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Charming as ever. Anyway, I think Zelly is our best shot.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Then I'll have to stay here.",
      "Try not to tick her off."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Then I'm off to Starbase 2.")
END_DIALOGUE_SCRIPT()

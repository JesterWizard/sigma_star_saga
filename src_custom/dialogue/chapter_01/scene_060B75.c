#include "dialogue_macros.h"

/* ROM 0x08060B75 */
DIALOGUE_SCRIPT(0x08060B75, scene_060B75)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I was right. The humans beat us here.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Humans?!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "That's an Earth symbol by the doorway, isn't it?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I don't believe it! This looks like some kind of mining operation!",
      "What could they be up to so far from Earth?"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Looks like your overlords have been doing some moonlighting!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Earth doesn't have overlords!",
      "Well, jeez, I don't know. Maybe we do."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "It would take a great deal of resources to secretly excavate a planet in Krill space.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Shush! I'm getting orders from the station.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Don't look at me!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You can talk through that creature?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I said quiet!",
      "...",
      "...",
      "Wow, I can't believe I have to explain this to a grown man.",
      "Ok, look...",
      "Female Krill have the ability to speak through their parasites when the fleet is overhead. Surely you've noticed the difference between male and female Krill?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Hard to miss.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Anyway, we're attacking the settlement.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "You feel like dishing out a little payback to your Earth friends?",
      "I'll set up a remote DATA PLUG here.",
      "Use it to SAVE your progress."
  )
END_DIALOGUE_SCRIPT()

#include "dialogue_macros.h"

/* ROM 0x080703D5 */
DIALOGUE_SCRIPT(0x080703D5, scene_0703D5)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "...",
      "Well, Psyme. Anything you want to ask me? About Tierney?",
      "My mission?"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I think I got the picture.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Are you surprised?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "No, not really. I've suspected you were soft on Earth for some time.",
      "But the sob story about your friends...",
      "Was that just cover?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I wish it was.",
      "But no, I was never convicted and sent to some space prison.",
      "The only reason I'm out here in the first place is to ease my guilty conscience."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I don't appreciate being lied to.",
      "But I suppose I did some lying of my own."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I don't have the anti-virus.",
      "This is from a beverage machine."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Holy Moley! You bluffed your way through that entire conversation?!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I didn't become an officer at my age by being a pushover.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "So where's the anti-virus now?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Let's think it through. We know Blune didn't get it.",
      "That leaves Professor Zart or Miss Keys.",
      "If it was Scarlet, she couldn't have gotten far, because she'd be on foot.",
      "If it was Zart, I'd need to plot every course and check Krill docking logs. Fun stuff."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "But we can't wait on finding the three Probes.",
      "Blune is probably on his way to find them first!"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Then you'd better go it solo. I'll find Miss Keys. I'll set up a frequency for the probes and our Comm system to share.",
      "That way we can use them as long range radios.",
      "The probes must be on the Fire, Ice, and Sand planets.",
      "Go find them!",
      "And Recker..."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yes?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'm still your superior. Lie to me again, and I'll kill you.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "This can't be good for our relationship.")
END_DIALOGUE_SCRIPT()

#include "dialogue_macros.h"

/* ROM 0x0805D06C */
DIALOGUE_SCRIPT(0x0805D06C, scene_05D06C)
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Commander Tierney, Sir!")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "At ease Recker.\nYou know I like to keep things informal.")
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Yes sir.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Let me be the first to congratulate you on a job well done. Those Krill monsters'll think twice after today.")
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Thank you sir.\nBut we lost good men.",
      "Some were old friends."
  )
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "I realize that, pilot.",
      "And that's why I'd like to offer you a chance to hit the Krill where it really hurts.",
      "But don't be hasty. You'll want to think this one through.",
      "The AEF has taken extensive measures to conceal what I'm about to tell you."
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "It won't leave this room sir.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "You're probably aware of the men we've lost gathering intel in Krill space.")
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Yes. Fifty men presumably captured or executed.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "What you don't know is that many of these men are alive, and now serve the Krill.",
      "The Krill use them in various low-risk capacities that profit from military training...to the Krill, their knowledge of Earth and the surrounding planets is invaluable.",
      "An opportunity exists that I want you to exploit."
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "And that is?")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Enlist into the Krill armada.")
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Become the enemy?",
      "Why?"
  )
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "We've engineered a scenario involving a Delta Class Krill Starbase.",
      "We can get you in.",
      "Once there, serve the Krill with diligence.",
      "Seek promotion through their ranks.",
      "Once you gather the intel we need, we'll extricate you immediately."
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "What exactly am I looking for?")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Complete specs on the Krill's new super weapon.",
      "Details about it are sketchy at best, but we believe it's a planet killer.",
      "We need you to find out what it is before it's too late."
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Then I have no choice.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "The clock is ticking.",
      "You're the only one I can trust with this mission.",
      "You need to decide how far you're willing to go for your planet."
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "All the way, sir, straight to the devil's door.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Good man.",
      "Get onto their Starbase, gain their confidence at the highest levels possible, and transmit any information you can about that weapon."
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Understood.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Better crack this open now.",
      "No telling what those devils drink, or if they even have bathrooms."
  )
END_DIALOGUE_SCRIPT()

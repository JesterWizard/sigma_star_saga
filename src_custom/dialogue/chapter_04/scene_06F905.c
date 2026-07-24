#include "dialogue_macros.h"

/* ROM 0x0806F905 */
DIALOGUE_SCRIPT(0x0806F905, scene_06F905)
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Hello Psyme. I've heard impressive things about you.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Cram it. In my hand is the anti-virus, created from your original strain.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "...")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I thought Blune stole it!")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "I...",
      "I didn't know about any anti-virus!",
      "You must believe me...the virus was the only thing in the lab!"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "That's because the anti-virus had not incubated yet. Looks like we're in the driver's seat now!")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker, I order you to seize that anti-virus!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Maybe we'll just call up the Tyrannical Overlord. I'm sure he'd love to have it.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Enough!",
      "You are relieved of duty! Return to Earth immediately with the anti-virus or you will be charged with high treason!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "And I suppose you will report me through the proper channels?",
      "The same way you arranged that shindig with a high ranking Krill officer?",
      "Blune, has he introduced you to anybody else around the office? Gone out for coffee?"
  )
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Cool down!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "This is all a solo act, isn't it?",
      "Does anyone else know you ordered me to destroy the fleet? Or gut that mine?",
      "Who are you accountable to?! No one!",
      "You can't threaten me without implicating yourself!"
  )
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "What do you want?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "There are still two planets out there. If you're willing to pull me out now you must know of at least one other.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "And what would you do with that information?",
      "Steal the bio weapon for yourself?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'd do that before I'd put it in the hands of you or the Krill Overlord.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Do not bring the anti-virus to the Krill. Can we at least agree on that?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "If he gives us the name of the fifth planet.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "I don't know which is the fifth planet, but...",
      "But I can explain the method you can use to find it."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Go on.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker. You realize this information will not help you. You do this and there's not a home to return to. Earth, Krill, everyone will want you dead.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Then I will hope for a stalemate. Talk.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Sigh...When the first human POW leaked information to Earth about the bio weapon, we planted probes on every unstable planet within range of the Krill's influence. With three of these Probes, we could triangulate based on long range resonances.",
      "Within days these probes were discovered and jammed by the Krill. We only managed to get locations of two planets in time, the Forest Planet and the Ice Planet.",
      "We immediately set up bases on each to find other ways of identifying all six planets."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "So these probes are already on the four planets we know of.",
      "Easy. We only need three after all."
  )
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Unfortunately one of them was the entry point for the Forest mining operation.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yeah, I pretty much blew that place up.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "So that's it then. We'll hold onto the anti-virus. If you're lying, it goes straight to the Tyrannical Overlord.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "The time for deception has passed. And Recker...",
      "You are on your own.",
      "Over, and out."
  )
END_DIALOGUE_SCRIPT()

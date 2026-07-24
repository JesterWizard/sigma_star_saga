#include "dialogue_macros.h"

/* ROM 0x08065411 */
DIALOGUE_SCRIPT(0x08065411, scene_065411)
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'm listening kid.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Things are going smoothly. I've allied with Blune and we've uncovered something strange on the Fire Planet.",
      "It sounds crazy, but the Forest and Fire worlds are somehow synchronized."
  )
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "What are the Krill doing with this information?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I don't know. They're having a closed conference in the Lab.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Can you get in there?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "They've decided to exclude me for some reason. But I suppose I could find a way to drop in.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "How close is your relationship with the Krill girl? Will she confide in you once the meeting is adjourned?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Blune might. But not Psyme or Commander Zelly.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "I need you in that room. Synchronized planets...this is what we've been waiting for.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'll get in there somehow.",
      "Recker out."
  )
END_DIALOGUE_SCRIPT()

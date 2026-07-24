#include "dialogue_macros.h"

/* ROM 0x0806204C */
DIALOGUE_SCRIPT(0x0806204C, scene_06204C)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Commander! Commander Tierney, come in!")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "You got my signal. Good.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I- I attacked...I had no choice!")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "It doesn't matter.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Doesn't Matter?")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "You got a look at their findings I take it?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yes, how did you know?")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Look Recker. Both Earth and the Krill are following the same set of clues. The Forest Planet was only a stepping stone.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "So you knew about the operation on that planet?")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Of course I did. I initiated it.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You sacrificed those people for this geological data!!")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Yes.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "But the data is in the hands of the Krill High Command, and I'm being transferred to another base as some kind of war hero! So things are looking pretty screwed up from my end!!")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Keep your voice down! Sure, we wanted the data.",
      "But now we just need to know what the Krill plan to do with it."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You're keeping me in the dark, Commander!")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "You know what you need to get the job done.",
      "That will have to be enough.",
      "Tierney out!!"
  )
END_DIALOGUE_SCRIPT()

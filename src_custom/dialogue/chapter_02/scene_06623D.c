#include "dialogue_macros.h"

/* ROM 0x0806623D */
DIALOGUE_SCRIPT(0x0806623D, scene_06623D)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Psyme reporting for active duty, Commander Zelly.",
      "I hope there are no hard feelings about what happened in front of the Overlord?"
  )
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "None.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Am I missing something?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Just stuff.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "I take it you are both well rested?",
      "High Command has completed their analysis of the Sync Data."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "And?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "It closely matches a third planet, but it is very distant.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "And they'd like us to check it out?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "You will both be transferred with the Sync Data. Share your findings with Commander Nomak. He has a starbase positioned there.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "We'll depart immediately.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Before you go, there is one additional detail.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Yes, Commander?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'm sure you've both heard by now. The spy. She was found and executed.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Executed?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Although I'm quite sure we'll have no further problems, I'd like to take the added precaution of transferring you both by cargo vessel rather than a conventional shuttle.",
      "It will help alleviate temptation in the event that the traitor was not working alone."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Is that really necessary?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "This way the list of potential traitors is reduced to three. You, Recker, and myself.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What about Blune and the crew?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "I plan to clean house once you depart.",
      "Report to the launch bay immediately."
  )
END_DIALOGUE_SCRIPT()

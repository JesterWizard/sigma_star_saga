#include "dialogue_macros.h"

/* ROM 0x0807140F */
DIALOGUE_SCRIPT(0x0807140F, scene_07140F)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme. This is urgent. Come in.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Go.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Commander Zelly is gone. They've erased all record of her existence.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Incredible. They are cleaning away every footprint. Pretty soon you and I will be the only ones left who know what's going on.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Has she been transferred? Or killed?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I noticed while working on SB2 that they have a detention section.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'll check it out.")
END_DIALOGUE_SCRIPT()

#include "dialogue_macros.h"

/* ROM 0x08065F1F */
DIALOGUE_SCRIPT(0x08065F1F, scene_065F1F)
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What are you doing at that locker?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Snooping. I heard there was a lockdown.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "You found something?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "This was inside. What is it?")
END_DIALOGUE_SCRIPT()

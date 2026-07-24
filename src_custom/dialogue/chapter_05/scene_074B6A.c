#include "dialogue_macros.h"

/* ROM 0x08074B6A */
DIALOGUE_SCRIPT(0x08074B6A, scene_074B6A)
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's done.",
      "All traces of the virus are gone."
  )
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Well done. Your payment will be forthcoming.")
END_DIALOGUE_SCRIPT()

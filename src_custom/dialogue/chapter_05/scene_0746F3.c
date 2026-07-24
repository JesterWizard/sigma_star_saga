#include "dialogue_macros.h"

/* ROM 0x080746F3 */
DIALOGUE_SCRIPT(0x080746F3, scene_0746F3)
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Come in Recker. You remember Professor Zart, don't you?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Zart! I thought you were dead!")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "Hello again Mister Recker.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You're working with Tierney? But I thought Blune stole the virus!")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "Of course he did. We had to part ways after your blonde girlfriend stole the anti-virus. But now that we have both...")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "We will destroy the original virus.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "But... But you commissioned the original virus yourself! You set up the laboratory on the Ice Planet! This makes no sense!")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Allied Earth Federation commissioned the virus. I just want the bio weapon. And now that I know the Krill will be eliminated by the hatching process, the virus only serves to threaten the weapon when I wield it! ")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "But if the Krill world is destroyed, the weapon serves no purpose!")
END_DIALOGUE_SCRIPT()

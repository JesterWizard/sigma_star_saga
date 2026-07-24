#include "dialogue_macros.h"

/* ROM 0x08076653 */
DIALOGUE_SCRIPT(0x08076653, scene_076653)
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Be a good girl and inform his highness there's been a change of plan.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Sir! It's Commander Zelly!",
      "She must have summoned him into a ship!"
  )
  TALK(SPEAKER_OVERLORD, SIDE_RIGHT, EXPR_NEUTRAL,
      "Take care of it.")
END_DIALOGUE_SCRIPT()

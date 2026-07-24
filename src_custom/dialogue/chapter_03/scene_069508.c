#include "dialogue_macros.h"

/* ROM 0x08069508 */
DIALOGUE_SCRIPT(0x08069508, scene_069508)
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Ah. You must be Recker.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yes sir! Reporting for duty sir!")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Looks like I'm your third boss since enlisting. That's a lot of transfers, especially for an offworld soldier.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "If I may interject sir! I have a message from Blune, my previous team leader. He is requesting a transfer from Starbase 2.")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Hah. That Zelly wench drives every man to wit's end. I'll see what strings I can pull.",
      "Ah, your pals are here."
  )
END_DIALOGUE_SCRIPT()

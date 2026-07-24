#include "dialogue_macros.h"

/* ROM 0x0805CBF9 */
DIALOGUE_SCRIPT(0x0805CBF9, scene_05CBF9)
  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "Hey Recker!",
      "I'm takin' clean up detail tonight.",
      "I'm gonna search every cockpit till I find me a dead alien!",
      "Krill-skin boots are in, sucka!",
      "Bam!"
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Uh, yeah. High five.")
END_DIALOGUE_SCRIPT()

#include "event_macros.h"

/* Script id 56 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805CBF9. */
EVENT_SCRIPT_REPLACEMENT(0x0805CBF9, scene_05CBF9)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "Hey Recker!",
      "I'm takin' clean up detail tonight.",
      "I'm gonna search every cockpit till I find me a dead alien!",
      "Krill-skin boots are in, sucka!",
      "Bam!"
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Uh, yeah. High five.")
  END()

END_EVENT_SCRIPT()

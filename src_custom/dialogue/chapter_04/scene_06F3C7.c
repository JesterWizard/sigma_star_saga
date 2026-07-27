#include "event_macros.h"

/* Script id 350 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806F3C7. */
EVENT_SCRIPT_REPLACEMENT(0x0806F3C7, scene_06F3C7)

  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker, I gave it to Commander Tierney.")
  END()

END_EVENT_SCRIPT()

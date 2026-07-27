#include "event_macros.h"

/* Script id 159 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08063D13. */
EVENT_SCRIPT_REPLACEMENT(0x08063D13, scene_063D13)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Good. This is exactly what we needed.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's done. Let's find two more spots similar to this one.")
  END()

END_EVENT_SCRIPT()

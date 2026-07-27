#include "event_macros.h"

/* Script id 131 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08061FA3. */
EVENT_SCRIPT_REPLACEMENT(0x08061FA3, scene_061FA3)

  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker! This is one way audio only. I need to speak with you in the COMM ROOM.")
  END()

END_EVENT_SCRIPT()

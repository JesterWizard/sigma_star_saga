#include "event_macros.h"

/* Script id 300 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806CABC. */
EVENT_SCRIPT_REPLACEMENT(0x0806CABC, scene_06CABC)

  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "This is it Recker! Don't even think about disengaging! Destroy every trace of them!")
  END()

END_EVENT_SCRIPT()

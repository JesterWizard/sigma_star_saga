#include "event_macros.h"

/* Script id 232 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08068F0F. */
EVENT_SCRIPT_REPLACEMENT(0x08068F0F, scene_068F0F)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker! Get over here!")
  END()

END_EVENT_SCRIPT()

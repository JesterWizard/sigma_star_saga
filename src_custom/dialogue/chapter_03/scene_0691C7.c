#include "event_macros.h"

/* Script id 237 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080691C7. */
EVENT_SCRIPT_REPLACEMENT(0x080691C7, scene_0691C7)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "The signal! Run ahead. Maybe you can stall the blast!")
  END()

END_EVENT_SCRIPT()

#include "event_macros.h"

/* Script id 348 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806F25C. */
EVENT_SCRIPT_REPLACEMENT(0x0806F25C, scene_06F25C)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Be sure to check the COMM ROOM too. A clue could be anywhere.")
  END()

END_EVENT_SCRIPT()

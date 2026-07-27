#include "event_macros.h"

/* Script id 297 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806C83E. */
EVENT_SCRIPT_REPLACEMENT(0x0806C83E, scene_06C83E)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker, wait up!")
  END()

END_EVENT_SCRIPT()

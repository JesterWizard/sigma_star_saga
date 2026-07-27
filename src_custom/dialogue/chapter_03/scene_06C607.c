#include "event_macros.h"

/* Script id 295 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806C607. */
EVENT_SCRIPT_REPLACEMENT(0x0806C607, scene_06C607)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "SCRAMBLE!!!")
  END()

END_EVENT_SCRIPT()

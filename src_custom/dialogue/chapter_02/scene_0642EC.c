#include "event_macros.h"

/* Script id 165 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080642EC. */
EVENT_SCRIPT_REPLACEMENT(0x080642EC, scene_0642EC)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "High Command is waiting for our report. Don't dilly dally.")
  END()

END_EVENT_SCRIPT()

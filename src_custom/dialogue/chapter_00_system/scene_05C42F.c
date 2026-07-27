#include "event_macros.h"

/* Script id 27 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C42F. */
EVENT_SCRIPT_REPLACEMENT(0x0805C42F, scene_05C42F)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Purple really works for you.")
  END()

END_EVENT_SCRIPT()

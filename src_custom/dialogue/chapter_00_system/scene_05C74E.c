#include "event_macros.h"

/* Script id 40 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C74E. */
EVENT_SCRIPT_REPLACEMENT(0x0805C74E, scene_05C74E)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "I still can't believe you survived that arena. You must be tough.")
  END()

END_EVENT_SCRIPT()

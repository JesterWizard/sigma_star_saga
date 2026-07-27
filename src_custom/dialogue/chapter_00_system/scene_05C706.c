#include "event_macros.h"

/* Script id 39 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C706. */
EVENT_SCRIPT_REPLACEMENT(0x0805C706, scene_05C706)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Some people shower with their parasite on. That strikes me as odd.")
  END()

END_EVENT_SCRIPT()

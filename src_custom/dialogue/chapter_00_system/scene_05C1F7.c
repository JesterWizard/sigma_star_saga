#include "event_macros.h"

/* Script id 17 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C1F7. */
EVENT_SCRIPT_REPLACEMENT(0x0805C1F7, scene_05C1F7)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Tomorrow's my day off. There's not much to do around here, though.")
  END()

END_EVENT_SCRIPT()

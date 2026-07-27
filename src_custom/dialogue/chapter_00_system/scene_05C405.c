#include "event_macros.h"

/* Script id 26 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C405. */
EVENT_SCRIPT_REPLACEMENT(0x0805C405, scene_05C405)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Hey Earthman. Keep up the good work.")
  END()

END_EVENT_SCRIPT()

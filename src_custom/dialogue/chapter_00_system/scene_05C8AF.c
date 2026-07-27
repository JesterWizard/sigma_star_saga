#include "event_macros.h"

/* Script id 46 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C8AF. */
EVENT_SCRIPT_REPLACEMENT(0x0805C8AF, scene_05C8AF)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Have you seen RyP? He's still got the project tool I need.")
  END()

END_EVENT_SCRIPT()

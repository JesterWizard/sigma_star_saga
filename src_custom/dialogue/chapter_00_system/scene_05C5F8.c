#include "event_macros.h"

/* Script id 34 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C5F8. */
EVENT_SCRIPT_REPLACEMENT(0x0805C5F8, scene_05C5F8)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "That's a nice gun, Earthman.")
  END()

END_EVENT_SCRIPT()

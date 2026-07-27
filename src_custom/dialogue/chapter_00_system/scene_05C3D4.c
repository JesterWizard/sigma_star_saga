#include "event_macros.h"

/* Script id 25 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C3D4. */
EVENT_SCRIPT_REPLACEMENT(0x0805C3D4, scene_05C3D4)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Those are nice boots. Are those regulation?")
  END()

END_EVENT_SCRIPT()

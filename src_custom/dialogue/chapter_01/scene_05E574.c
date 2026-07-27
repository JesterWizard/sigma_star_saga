#include "event_macros.h"

/* Script id 78 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805E574. */
EVENT_SCRIPT_REPLACEMENT(0x0805E574, scene_05E574)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Skinning room's up ahead. No messing around.")
  END()

END_EVENT_SCRIPT()

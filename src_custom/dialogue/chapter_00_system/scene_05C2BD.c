#include "event_macros.h"

/* Script id 20 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C2BD. */
EVENT_SCRIPT_REPLACEMENT(0x0805C2BD, scene_05C2BD)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Were you the one who went in my quarters? You shouldn't just wander into places like that.")
  END()

END_EVENT_SCRIPT()

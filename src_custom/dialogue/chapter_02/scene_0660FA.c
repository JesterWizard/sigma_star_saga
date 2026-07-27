#include "event_macros.h"

/* Script id 185 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080660FA. */
EVENT_SCRIPT_REPLACEMENT(0x080660FA, scene_0660FA)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "Did you hear? They just grabbed the traitor!")
  END()

END_EVENT_SCRIPT()

#include "event_macros.h"

/* Script id 175 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080653B4. */
EVENT_SCRIPT_REPLACEMENT(0x080653B4, scene_0653B4)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "Giant monster? Can't say I'm surprised. Space is full of strange and wonderful things.")
  END()

END_EVENT_SCRIPT()

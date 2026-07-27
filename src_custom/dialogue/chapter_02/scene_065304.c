#include "event_macros.h"

/* Script id 173 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08065304. */
EVENT_SCRIPT_REPLACEMENT(0x08065304, scene_065304)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "Excavation of Earth? Yeah, my grandfather was assigned to that mission. He could never explain what they found though.")
  END()

END_EVENT_SCRIPT()

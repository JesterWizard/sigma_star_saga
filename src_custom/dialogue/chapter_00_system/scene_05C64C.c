#include "event_macros.h"

/* Script id 36 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C64C. */
EVENT_SCRIPT_REPLACEMENT(0x0805C64C, scene_05C64C)

  TALK(SPEAKER_EK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Watch out for Krill females. They're a handful.")
  END()

END_EVENT_SCRIPT()

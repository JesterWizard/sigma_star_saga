#include "event_macros.h"

/* Script id 428 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0807529E. */
EVENT_SCRIPT_REPLACEMENT(0x0807529E, scene_07529E)

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Come on!")
  END()

END_EVENT_SCRIPT()

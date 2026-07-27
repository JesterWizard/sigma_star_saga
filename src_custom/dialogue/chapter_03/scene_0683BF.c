#include "event_macros.h"

/* Script id 213 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080683BF. */
EVENT_SCRIPT_REPLACEMENT(0x080683BF, scene_0683BF)

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Just when I thought men couldn't stoop any lower!")
  END()

END_EVENT_SCRIPT()

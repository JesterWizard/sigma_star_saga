#include "event_macros.h"

/* Script id 189 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806620B. */
EVENT_SCRIPT_REPLACEMENT(0x0806620B, scene_06620B)

  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker, good timing. I just summoned Psyme.")
  END()

END_EVENT_SCRIPT()

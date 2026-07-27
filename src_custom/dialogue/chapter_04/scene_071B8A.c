#include "event_macros.h"

/* Script id 373 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08071B8A. */
EVENT_SCRIPT_REPLACEMENT(0x08071B8A, scene_071B8A)

  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "You make a lovely couple.",
      "I wish you both a safe voyage."
  )
  END()

END_EVENT_SCRIPT()

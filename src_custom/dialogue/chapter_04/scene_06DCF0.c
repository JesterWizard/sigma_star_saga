#include "event_macros.h"

/* Script id 315 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806DCF0. */
EVENT_SCRIPT_REPLACEMENT(0x0806DCF0, scene_06DCF0)

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Hold on! Now that we're alone, I need to know what's going on between you and that alien girl.",
      "Is cuddling up to her part of your assignment?"
  )
  END()

END_EVENT_SCRIPT()

#include "event_macros.h"

/* Script id 204 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08067DA4. */
EVENT_SCRIPT_REPLACEMENT(0x08067DA4, scene_067DA4)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What?!",
      "How could you do that?!",
      "Our necks are on the chopping block as it is."
  )
  END()

END_EVENT_SCRIPT()

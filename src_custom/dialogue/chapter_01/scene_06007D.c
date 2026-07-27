#include "event_macros.h"

/* Script id 103 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806007D. */
EVENT_SCRIPT_REPLACEMENT(0x0806007D, scene_06007D)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "You need to use that NAV computer.",
      "Drop us out of orbit and take us down by choosing LAND and pressing the A Button. "
  )
  END()

END_EVENT_SCRIPT()

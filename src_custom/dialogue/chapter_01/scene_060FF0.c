#include "event_macros.h"

/* Script id 114 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08060FF0. */
EVENT_SCRIPT_REPLACEMENT(0x08060FF0, scene_060FF0)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "We use these as lures to attract the nearest ship. Don't step into the circle unless you're ready for a battle. I'll be right behind you when you do.")
  END()

END_EVENT_SCRIPT()

#include "event_macros.h"

/* Script id 125 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080618A5. */
EVENT_SCRIPT_REPLACEMENT(0x080618A5, scene_0618A5)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "I remember my first parasite...bit down on my head like a bugger!",
      "A good slam against the wall takes care of that!"
  )
  END()

END_EVENT_SCRIPT()

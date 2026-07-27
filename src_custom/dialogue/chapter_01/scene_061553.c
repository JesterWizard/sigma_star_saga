#include "event_macros.h"

/* Script id 119 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08061553. */
EVENT_SCRIPT_REPLACEMENT(0x08061553, scene_061553)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "This DATA PLUG terminal is for SAVING your data. If we lost power unexpectedly, you'd lose all of your progress, and nobody wants that. All you have to do is step on that plate and follow the instructions from there. Save often will ya!")
  END()

END_EVENT_SCRIPT()

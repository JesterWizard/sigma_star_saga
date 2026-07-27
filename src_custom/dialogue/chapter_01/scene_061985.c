#include "event_macros.h"

/* Script id 127 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08061985. */
EVENT_SCRIPT_REPLACEMENT(0x08061985, scene_061985)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Now that you have a Krill parasite, you can run faster than ever before!  Hold the B Button to run!")
  END()

END_EVENT_SCRIPT()

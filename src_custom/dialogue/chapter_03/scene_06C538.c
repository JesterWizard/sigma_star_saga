#include "event_macros.h"

/* Script id 292 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806C538. */
EVENT_SCRIPT_REPLACEMENT(0x0806C538, scene_06C538)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "All! I REPEAT!! ALL EARTH SHIPS APPROACHING!!")
  END()

END_EVENT_SCRIPT()

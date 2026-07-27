#include "event_macros.h"

/* Script id 120 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08061646. */
EVENT_SCRIPT_REPLACEMENT(0x08061646, scene_061646)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Through here is the commander's room. But take my advice and stay out of the COMM ROOM...Just because it's unlocked now doesn't make it your business.")
  END()

END_EVENT_SCRIPT()

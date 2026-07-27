#include "event_macros.h"

/* Script id 362 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08071069. */
EVENT_SCRIPT_REPLACEMENT(0x08071069, scene_071069)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Commander Zelly? Nope. Commander Ammer has always been the commander of Starbase 2. Was when I joined up.")
  END()

END_EVENT_SCRIPT()

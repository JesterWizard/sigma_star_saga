#include "event_macros.h"

/* Script id 260 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806A0E4. */
EVENT_SCRIPT_REPLACEMENT(0x0806A0E4, scene_06A0E4)

  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Before I thinned the ranks, one of my crewmen went missing.")
  END()

END_EVENT_SCRIPT()

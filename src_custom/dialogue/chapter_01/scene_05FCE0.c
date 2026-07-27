#include "event_macros.h"

/* Script id 101 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805FCE0. */
EVENT_SCRIPT_REPLACEMENT(0x0805FCE0, scene_05FCE0)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Whew.....they sure don't make 'em like that on Earth.")
  END()

END_EVENT_SCRIPT()

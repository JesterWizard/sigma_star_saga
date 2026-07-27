#include "event_macros.h"

/* Script id 147 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080630BC. */
EVENT_SCRIPT_REPLACEMENT(0x080630BC, scene_0630BC)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "Why is the High Command so interested in geology lately? It seems like every starbase is positioned near some prime planet. Weird.")
  END()

END_EVENT_SCRIPT()

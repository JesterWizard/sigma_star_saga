#include "event_macros.h"

/* Script id 387 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0807254E. */
EVENT_SCRIPT_REPLACEMENT(0x0807254E, scene_07254E)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Aren't you going to sit in the chair? It's yours, right?")
  END()

END_EVENT_SCRIPT()

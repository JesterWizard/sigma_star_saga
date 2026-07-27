#include "event_macros.h"

/* Script id 210 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08068319. */
EVENT_SCRIPT_REPLACEMENT(0x08068319, scene_068319)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I heard shots!")
  END()

END_EVENT_SCRIPT()

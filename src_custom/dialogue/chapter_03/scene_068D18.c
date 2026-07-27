#include "event_macros.h"

/* Script id 220 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08068D18. */
EVENT_SCRIPT_REPLACEMENT(0x08068D18, scene_068D18)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "We're digging out these snowpiles so we can reach the starbase once it arrives.")
  END()

END_EVENT_SCRIPT()

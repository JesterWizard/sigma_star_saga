#include "event_macros.h"

/* Script id 105 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080603CC. */
EVENT_SCRIPT_REPLACEMENT(0x080603CC, scene_0603CC)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Ha! This will be rich.",
      "Try to keep up."
  )
  END()

END_EVENT_SCRIPT()

#include "event_macros.h"

/* Script id 396 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08072F2D. */
EVENT_SCRIPT_REPLACEMENT(0x08072F2D, scene_072F2D)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I saw it all from the starbase! What is going on?!")
  END()

END_EVENT_SCRIPT()

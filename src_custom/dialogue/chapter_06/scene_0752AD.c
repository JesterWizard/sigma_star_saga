#include "event_macros.h"

/* Script id 429 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080752AD. */
EVENT_SCRIPT_REPLACEMENT(0x080752AD, scene_0752AD)

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Let's go!")
  END()

END_EVENT_SCRIPT()

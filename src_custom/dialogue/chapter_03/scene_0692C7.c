#include "event_macros.h"

/* Script id 241 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080692C7. */
EVENT_SCRIPT_REPLACEMENT(0x080692C7, scene_0692C7)

  TALK(SPEAKER_EK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Look at this! What do we have here?",
      "A pretty little Earth female!"
  )
  END()

END_EVENT_SCRIPT()

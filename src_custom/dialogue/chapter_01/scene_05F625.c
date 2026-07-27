#include "event_macros.h"

/* Script id 91 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805F625. */
EVENT_SCRIPT_REPLACEMENT(0x0805F625, scene_05F625)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Not bad Earthman!",
      "Commander Bloss will want to see you!"
  )
  END()

END_EVENT_SCRIPT()

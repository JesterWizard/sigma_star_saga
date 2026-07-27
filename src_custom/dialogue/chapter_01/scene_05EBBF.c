#include "event_macros.h"

/* Script id 84 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805EBBF. */
EVENT_SCRIPT_REPLACEMENT(0x0805EBBF, scene_05EBBF)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Commander Bloss hasn't given you launch bay clearance.",
      "You'll need to turn around."
  )
  END()

END_EVENT_SCRIPT()

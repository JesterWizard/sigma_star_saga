#include "event_macros.h"

/* Script id 252 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08069A18. */
EVENT_SCRIPT_REPLACEMENT(0x08069A18, scene_069A18)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Normally we can taxi between starbases at any time. One of the perks of being in the upper ranks.",
      "You should take advantage of it once the shuttle gets back."
  )
  END()

END_EVENT_SCRIPT()

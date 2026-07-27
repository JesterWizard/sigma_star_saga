#include "event_macros.h"

/* Script id 139 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080626EC. */
EVENT_SCRIPT_REPLACEMENT(0x080626EC, scene_0626EC)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "37...",
      "38...",
      "Wait, where was I?",
      "Don't break my concentration, Earthman!"
  )
  END()

END_EVENT_SCRIPT()

#include "event_macros.h"

/* Script id 146 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08063067. */
EVENT_SCRIPT_REPLACEMENT(0x08063067, scene_063067)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "Commander Zelly will kill me if she catches us talking. Keep walking Earthman.")
  END()

END_EVENT_SCRIPT()

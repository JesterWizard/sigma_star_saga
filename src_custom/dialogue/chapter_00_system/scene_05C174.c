#include "event_macros.h"

/* Script id 15 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C174. */
EVENT_SCRIPT_REPLACEMENT(0x0805C174, scene_05C174)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Everything looks so peaceful from up here. I wish the war was over.")
  END()

END_EVENT_SCRIPT()

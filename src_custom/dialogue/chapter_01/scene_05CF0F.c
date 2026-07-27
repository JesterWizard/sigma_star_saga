#include "event_macros.h"

/* Script id 64 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805CF0F. */
EVENT_SCRIPT_REPLACEMENT(0x0805CF0F, scene_05CF0F)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "Through here is the laboratory, but nobody uses it these days. We should make it a game room.")
  END()

END_EVENT_SCRIPT()

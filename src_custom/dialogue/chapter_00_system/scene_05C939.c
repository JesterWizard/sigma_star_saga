#include "event_macros.h"

/* Script id 48 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C939. */
EVENT_SCRIPT_REPLACEMENT(0x0805C939, scene_05C939)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "I knew an Earthman once. He was shot down in the last assault, though.")
  END()

END_EVENT_SCRIPT()

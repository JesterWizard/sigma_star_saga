#include "event_macros.h"

/* Script id 22 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C345. */
EVENT_SCRIPT_REPLACEMENT(0x0805C345, scene_05C345)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Hey, want to play a game? What? You already are? I don't get it.")
  END()

END_EVENT_SCRIPT()

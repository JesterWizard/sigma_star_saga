#include "event_macros.h"

/* Script id 44 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C83D. */
EVENT_SCRIPT_REPLACEMENT(0x0805C83D, scene_05C83D)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Have you met Lt. Blune yet? You remind me of him.")
  END()

END_EVENT_SCRIPT()

#include "event_macros.h"

/* Script id 63 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805CEB4. */
EVENT_SCRIPT_REPLACEMENT(0x0805CEB4, scene_05CEB4)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "The Krill were working on SMART BOMB technology. If only Earth had weapons like that.")
  END()

END_EVENT_SCRIPT()

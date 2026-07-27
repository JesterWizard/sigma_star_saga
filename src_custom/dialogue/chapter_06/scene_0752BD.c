#include "event_macros.h"

/* Script id 430 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080752BD. */
EVENT_SCRIPT_REPLACEMENT(0x080752BD, scene_0752BD)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Sit tight. I want to check things out.")
  END()

END_EVENT_SCRIPT()

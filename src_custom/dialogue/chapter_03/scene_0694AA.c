#include "event_macros.h"

/* Script id 244 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080694AA. */
EVENT_SCRIPT_REPLACEMENT(0x080694AA, scene_0694AA)

  TALK(SPEAKER_EK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Bah well, best not to keep the boss waiting.",
      "You'll find Commander Nomak in his office."
  )
  END()

END_EVENT_SCRIPT()

#include "event_macros.h"

/* Script id 71 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805D8D0. */
EVENT_SCRIPT_REPLACEMENT(0x0805D8D0, scene_05D8D0)

  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Sorry pilot.",
      "It had to be a lethal dose.",
      "Go out proud, son, like your friends.",
      "Make the ultimate sacrifice."
  )
  END()

END_EVENT_SCRIPT()

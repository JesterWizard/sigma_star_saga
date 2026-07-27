#include "event_macros.h"

/* Script id 398 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0807361E. */
EVENT_SCRIPT_REPLACEMENT(0x0807361E, scene_07361E)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Iot. This water planet they stole the Living Flesh from?",
      "What was its name?"
  )
  TALK(SPEAKER_IOT, SIDE_RIGHT, EXPR_NEUTRAL,
      "E a r t h .")
  END()

END_EVENT_SCRIPT()

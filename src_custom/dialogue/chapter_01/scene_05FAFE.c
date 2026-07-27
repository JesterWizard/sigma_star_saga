#include "event_macros.h"

/* Script id 97 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805FAFE. */
EVENT_SCRIPT_REPLACEMENT(0x0805FAFE, scene_05FAFE)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Excuse me?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Your parasite. Let's take a look.")
  END()

END_EVENT_SCRIPT()

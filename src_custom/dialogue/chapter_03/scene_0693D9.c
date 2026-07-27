#include "event_macros.h"

/* Script id 243 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080693D9. */
EVENT_SCRIPT_REPLACEMENT(0x080693D9, scene_0693D9)

  TALK(SPEAKER_EK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Whup! Sounds like the site blasting's begun. Good thing you didn't dawdle, eh?",
      "Miss Psyme, be a good girl and wash that pet before giving her to Nomak. ",
      "Get some of that Earth stink off her."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Idiots.")
  END()

END_EVENT_SCRIPT()

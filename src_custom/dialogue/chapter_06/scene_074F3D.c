#include "event_macros.h"

/* Script id 424 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08074F3D. */
EVENT_SCRIPT_REPLACEMENT(0x08074F3D, scene_074F3D)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_NEUTRAL,
      "We found Tierney's body. What remained of it anyway.",
      "Tell us how you did it."
  )
  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_NEUTRAL,
      "Need I remind you we have direct control over your body?",
      "Talk, you!!"
  )
  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_NEUTRAL,
      "Forget it. He won't talk.",
      "Shut him down so we can go."
  )
  END()

END_EVENT_SCRIPT()

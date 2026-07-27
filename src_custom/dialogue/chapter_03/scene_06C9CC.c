#include "event_macros.h"

/* Script id 299 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806C9CC. */
EVENT_SCRIPT_REPLACEMENT(0x0806C9CC, scene_06C9CC)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "When you come back we'll make things right! We can start over together!",
      "Promise me!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I...",
      "I will!",
      "But not for revenge...and not because of someone's orders!",
      "I won't let you go alone! I'll do it for you."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "We're out of time, GO!!")
  END()

END_EVENT_SCRIPT()

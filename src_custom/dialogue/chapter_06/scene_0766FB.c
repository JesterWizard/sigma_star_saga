#include "event_macros.h"

/* Script id 438 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080766FB. */
EVENT_SCRIPT_REPLACEMENT(0x080766FB, scene_0766FB)

  TALK(SPEAKER_OVERLORD, SIDE_RIGHT, EXPR_NEUTRAL,
      "The moment is at hand! The Sleepers Awaken!")
  END()

END_EVENT_SCRIPT()

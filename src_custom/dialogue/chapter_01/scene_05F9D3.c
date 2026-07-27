#include "event_macros.h"

/* Script id 94 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805F9D3. */
EVENT_SCRIPT_REPLACEMENT(0x0805F9D3, scene_05F9D3)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What's with you Earthman?",
      "Never seen anyone get skinned before?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "...er...")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "So you're the new guy, huh?")
  END()

END_EVENT_SCRIPT()

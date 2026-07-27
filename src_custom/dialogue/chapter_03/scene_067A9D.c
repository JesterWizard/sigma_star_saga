#include "event_macros.h"

/* Script id 202 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08067A9D. */
EVENT_SCRIPT_REPLACEMENT(0x08067A9D, scene_067A9D)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I shouldn't be so surprised, but here I am, standing in the presence of yet another unexplainable human settlement!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's not a mining operation. No heavy equipment or displaced land.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Any tremors?")
  END()

END_EVENT_SCRIPT()

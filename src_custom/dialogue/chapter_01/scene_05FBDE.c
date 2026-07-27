#include "event_macros.h"

/* Script id 100 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805FBDE. */
EVENT_SCRIPT_REPLACEMENT(0x0805FBDE, scene_05FBDE)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "And sensitive too! I'll have to learn to play nice.",
      "It's Psyme in case you're wondering.",
      "Easy to remember. Rhymes with Rhyme."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "My name. Bloss said you're tagging along.",
      "Be at the landing hatch in five minutes.",
      "It's next to the launch bay."
  )
  END()

END_EVENT_SCRIPT()

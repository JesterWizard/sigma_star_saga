#include "event_macros.h"

/* Script id 405 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08073AEE. */
EVENT_SCRIPT_REPLACEMENT(0x08073AEE, scene_073AEE)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Where are your sisters going?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Who cares?! Let's get out of here!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "It's only a matter of time before these tremors reach lethal proportions!",
      "Hurry!"
  )
  END()

END_EVENT_SCRIPT()

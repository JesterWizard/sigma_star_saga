#include "event_macros.h"

/* Script id 452 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080770EA. */
EVENT_SCRIPT_REPLACEMENT(0x080770EA, scene_0770EA)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Is that...the Overlord?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "He's merged with it!",
      "That's the weakest spot!",
      "Destroy it!"
  )
  END()

END_EVENT_SCRIPT()

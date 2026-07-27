#include "event_macros.h"

/* Script id 404 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08073A8D. */
EVENT_SCRIPT_REPLACEMENT(0x08073A8D, scene_073A8D)

  TALK(SPEAKER_IOT, SIDE_RIGHT, EXPR_NEUTRAL,
      "You must go.",
      "The child stirs. Soon all will end."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "You heard him, let's get out of here!")
  END()

END_EVENT_SCRIPT()

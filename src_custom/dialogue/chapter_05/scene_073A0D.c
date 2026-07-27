#include "event_macros.h"

/* Script id 403 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08073A0D. */
EVENT_SCRIPT_REPLACEMENT(0x08073A0D, scene_073A0D)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What is this?")
  TALK(SPEAKER_IOT, SIDE_RIGHT, EXPR_NEUTRAL,
      "An essential mechanism used in life by the IOT.",
      "In times of need it may aid you.",
      "Nothing more can I do."
  )
  END()

END_EVENT_SCRIPT()

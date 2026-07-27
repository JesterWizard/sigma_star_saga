#include "event_macros.h"

/* Script id 293 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806C56C. */
EVENT_SCRIPT_REPLACEMENT(0x0806C56C, scene_06C56C)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "They've sent their entire fleet to destroy us! We can't repel this many humans!")
  END()

END_EVENT_SCRIPT()

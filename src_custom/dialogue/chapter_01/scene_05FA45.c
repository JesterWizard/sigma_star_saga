#include "event_macros.h"

/* Script id 95 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805FA45. */
EVENT_SCRIPT_REPLACEMENT(0x0805FA45, scene_05FA45)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "You're not much to look at.",
      "But my parasite seems to like you."
  )
  END()

END_EVENT_SCRIPT()

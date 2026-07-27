#include "event_macros.h"

/* Script id 302 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806CB45. */
EVENT_SCRIPT_REPLACEMENT(0x0806CB45, scene_06CB45)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I can't stay! I have to get out of here.")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Calm down, Recker. It'll pass...")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I just destroyed the last line of defense for my people!")
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "You are safe here.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Ek is dead.",
      "I found his body in the launch bay."
  )
  END()

END_EVENT_SCRIPT()

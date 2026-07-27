#include "event_macros.h"

/* Script id 332 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E48B. */
EVENT_SCRIPT_REPLACEMENT(0x0806E48B, scene_06E48B)

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Holy Toledo! What have you done Recker?!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Nothing! Just a kiss!")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "You'll have to take her to a pool of water. Somewhere clean. The process can run its course there.",
      "I can't keep her on the station in her condition.",
      "Medic!"
  )
  END()

END_EVENT_SCRIPT()

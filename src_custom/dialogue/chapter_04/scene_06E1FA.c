#include "event_macros.h"

/* Script id 328 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E1FA. */
EVENT_SCRIPT_REPLACEMENT(0x0806E1FA, scene_06E1FA)

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Wowee!! A living dinosaur bone!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Told ya!")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Let me extract a sample...")
  END()

END_EVENT_SCRIPT()

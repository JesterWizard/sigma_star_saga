#include "event_macros.h"

/* Script id 207 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08067F70. */
EVENT_SCRIPT_REPLACEMENT(0x08067F70, scene_067F70)

  TALK(SPEAKER_UNKNOWN, SIDE_RIGHT, EXPR_NEUTRAL,
      "Help! Help me! They're everywhere!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Hang on!")
  TALK(SPEAKER_UNKNOWN, SIDE_RIGHT, EXPR_NEUTRAL,
      "Aim for the head!")
  END()

END_EVENT_SCRIPT()

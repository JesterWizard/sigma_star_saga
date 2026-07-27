#include "event_macros.h"

/* Script id 330 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E2A0. */
EVENT_SCRIPT_REPLACEMENT(0x0806E2A0, scene_06E2A0)

  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "Did you find the Genome?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Right here.")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "Let's get started. We'll breed the virus in one tank, and the anti-virus in the other.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Don't worry Psyme, I didn't put the moves on loverboy.",
      "Err Psyme, are you all right?"
  )
  END()

END_EVENT_SCRIPT()

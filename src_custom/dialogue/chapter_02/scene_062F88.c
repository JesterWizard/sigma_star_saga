#include "event_macros.h"

/* Script id 144 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08062F88. */
EVENT_SCRIPT_REPLACEMENT(0x08062F88, scene_062F88)

  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker...come in. Find the COMM ROOM and contact me ASAP.",
      "Tierney out!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Great. I have no access to that room. Now what am I going to do?")
  END()

END_EVENT_SCRIPT()

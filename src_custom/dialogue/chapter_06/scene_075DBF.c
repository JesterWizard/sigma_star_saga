#include "event_macros.h"

/* Script id 434 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08075DBF. */
EVENT_SCRIPT_REPLACEMENT(0x08075DBF, scene_075DBF)

  TALK(SPEAKER_OVERLORD_SHADOW, SIDE_RIGHT, EXPR_NEUTRAL,
      "Welcome.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "...",
      "Who's there?"
  )
  TALK(SPEAKER_OVERLORD_SHADOW, SIDE_RIGHT, EXPR_NEUTRAL,
      "You've managed to infiltrate my private chambers Mister Recker.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "So you're the all powerful Overlord?")
  END()

END_EVENT_SCRIPT()

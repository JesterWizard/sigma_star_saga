#include "event_macros.h"

/* Script id 333 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E574. */
EVENT_SCRIPT_REPLACEMENT(0x0806E574, scene_06E574)

  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "Take Psyme to the launch bay and prepare a ship for Recker. He's adopted some...er...new responsibilities.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Wha...How could this happen?")
  END()

END_EVENT_SCRIPT()

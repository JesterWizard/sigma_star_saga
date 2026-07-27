#include "event_macros.h"

/* Script id 180 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08065B7A. */
EVENT_SCRIPT_REPLACEMENT(0x08065B7A, scene_065B7A)

  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Yes, Overlord?")
  TALK(SPEAKER_OVERLORD_SHADOW, SIDE_RIGHT, EXPR_NEUTRAL,
      "Psyme has informed me of a traitor in your midst.")
  END()

END_EVENT_SCRIPT()

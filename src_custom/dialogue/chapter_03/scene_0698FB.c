#include "event_macros.h"

/* Script id 249 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080698FB. */
EVENT_SCRIPT_REPLACEMENT(0x080698FB, scene_0698FB)

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "What are you doing?! Don't leave me with this lunatic!")
  END()

END_EVENT_SCRIPT()

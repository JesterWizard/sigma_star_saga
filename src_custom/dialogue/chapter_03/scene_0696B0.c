#include "event_macros.h"

/* Script id 246 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080696B0. */
EVENT_SCRIPT_REPLACEMENT(0x080696B0, scene_0696B0)

  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Who's the pink skin?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "This is Scarlet, Commander. Just a little thank you for the promotion.")
  END()

END_EVENT_SCRIPT()

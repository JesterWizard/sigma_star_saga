#include "event_macros.h"

/* Script id 258 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08069DD4. */
EVENT_SCRIPT_REPLACEMENT(0x08069DD4, scene_069DD4)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What?!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'll go alone. Just try to keep Scarlet out of trouble, would you? I think you owe her that much.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What do I owe her for?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Wake up!! If not for Scarlet, you'd still be buried in snow! Zelly tried to bump you off, don't you get it?!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "sigh...",
      "Very well."
  )
  END()

END_EVENT_SCRIPT()

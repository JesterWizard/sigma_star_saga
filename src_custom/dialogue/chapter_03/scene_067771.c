#include "event_macros.h"

/* Script id 197 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08067771. */
EVENT_SCRIPT_REPLACEMENT(0x08067771, scene_067771)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "You'd better not be referring to my kiss.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Err...",
      "I'll get the hatch."
  )
  END()

END_EVENT_SCRIPT()

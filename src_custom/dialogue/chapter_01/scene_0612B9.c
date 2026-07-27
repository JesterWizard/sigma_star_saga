#include "event_macros.h"

/* Script id 117 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080612B9. */
EVENT_SCRIPT_REPLACEMENT(0x080612B9, scene_0612B9)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "The SCANNING TOOL should be easy enough for even you.",
      "Press the L or R BUTTON to switch from your GUN to that TOOL.",
      "The scanner will activate automatically.",
      "Give it a try on the way back to the starbase.",
      "I'll see you there."
  )
  END()

END_EVENT_SCRIPT()

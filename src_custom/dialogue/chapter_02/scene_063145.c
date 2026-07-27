#include "event_macros.h"

/* Script id 148 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08063145. */
EVENT_SCRIPT_REPLACEMENT(0x08063145, scene_063145)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "I have a friend working on Starbase 4. She led an expedition into an alien pyramid and hasn't been seen since.",
      "I hope she's OK."
  )
  END()

END_EVENT_SCRIPT()

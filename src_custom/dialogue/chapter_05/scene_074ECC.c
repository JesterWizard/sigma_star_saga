#include "event_macros.h"

/* Script id 420 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08074ECC. */
EVENT_SCRIPT_REPLACEMENT(0x08074ECC, scene_074ECC)

  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "The power... it is... meant for me...",
      "...",
      "..."
  )
  END()

END_EVENT_SCRIPT()

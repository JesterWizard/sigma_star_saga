#include "event_macros.h"

/* Script id 304 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806CE66. */
EVENT_SCRIPT_REPLACEMENT(0x0806CE66, scene_06CE66)

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Hold on! I have to tell you something first...",
      "I took a tissue sample from Ek. It was nothing like the tissue we were given to base our virus on. Allied Earth Federation lied to us."
  )
  END()

END_EVENT_SCRIPT()

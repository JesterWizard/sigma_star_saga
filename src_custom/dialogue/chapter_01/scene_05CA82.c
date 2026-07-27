#include "event_macros.h"

/* Script id 53 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805CA82. */
EVENT_SCRIPT_REPLACEMENT(0x0805CA82, scene_05CA82)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "Nice shootin' pal! Saw it all on the scope!")
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Thanks.")
  END()

END_EVENT_SCRIPT()

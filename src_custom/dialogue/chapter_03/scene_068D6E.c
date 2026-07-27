#include "event_macros.h"

/* Script id 221 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08068D6E. */
EVENT_SCRIPT_REPLACEMENT(0x08068D6E, scene_068D6E)

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "You gotta place TEN FLARES. Find dry spots and jam them pointy side down.")
  END()

END_EVENT_SCRIPT()

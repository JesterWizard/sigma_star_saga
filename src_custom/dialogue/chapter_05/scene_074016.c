#include "event_macros.h"

/* Script id 407 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08074016. */
EVENT_SCRIPT_REPLACEMENT(0x08074016, scene_074016)

  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "You did the right thing Recker.")
  END()

END_EVENT_SCRIPT()

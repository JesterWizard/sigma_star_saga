#include "event_macros.h"

/* Script id 11 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C07D. */
EVENT_SCRIPT_REPLACEMENT(0x0805C07D, scene_05C07D)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Cleanest base in the galaxy. Really, ask anyone.")
  END()

END_EVENT_SCRIPT()

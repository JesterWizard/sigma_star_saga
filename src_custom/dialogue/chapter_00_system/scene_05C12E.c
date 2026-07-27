#include "event_macros.h"

/* Script id 14 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805C12E. */
EVENT_SCRIPT_REPLACEMENT(0x0805C12E, scene_05C12E)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "We have standard issued uniforms. Yours is much fancier, though.")
  END()

END_EVENT_SCRIPT()

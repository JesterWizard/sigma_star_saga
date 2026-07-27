#include "event_macros.h"

/* Script id 52 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805CA2F. */
EVENT_SCRIPT_REPLACEMENT(0x0805CA2F, scene_05CA2F)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "You're a hero Lieutenant!",
      "Stay suited up. Commander wants you straight away."
  )
  END()

END_EVENT_SCRIPT()

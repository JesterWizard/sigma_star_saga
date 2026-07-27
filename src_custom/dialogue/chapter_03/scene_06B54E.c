#include "event_macros.h"

/* Script id 278 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806B54E. */
EVENT_SCRIPT_REPLACEMENT(0x0806B54E, scene_06B54E)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Sorry. Commander wants you on the surface looking for the Black Box. Shuttles are restricted for the time being.")
  END()

END_EVENT_SCRIPT()

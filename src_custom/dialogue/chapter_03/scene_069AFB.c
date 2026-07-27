#include "event_macros.h"

/* Script id 254 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08069AFB. */
EVENT_SCRIPT_REPLACEMENT(0x08069AFB, scene_069AFB)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Commander Nomak has some kind of special boots. When we first arrived, he went for a jog in the snow to try them out.",
      "Yup, those sure were some snappy boots."
  )
  END()

END_EVENT_SCRIPT()

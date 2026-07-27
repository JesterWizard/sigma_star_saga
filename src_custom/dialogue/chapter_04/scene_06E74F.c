#include "event_macros.h"

/* Script id 336 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E74F. */
EVENT_SCRIPT_REPLACEMENT(0x0806E74F, scene_06E74F)

  TALK(SPEAKER_MEDIC, SIDE_RIGHT, EXPR_NEUTRAL,
      "We put Psyme on board. Two of us will travel with you.",
      "I recommend the Forest Planet if you're looking for pure water appropriate for this kind of thing."
  )
  END()

END_EVENT_SCRIPT()

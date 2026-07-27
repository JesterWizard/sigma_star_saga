#include "event_macros.h"

/* Script id 337 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E7EF. */
EVENT_SCRIPT_REPLACEMENT(0x0806E7EF, scene_06E7EF)

  TALK(SPEAKER_MEDIC, SIDE_RIGHT, EXPR_NEUTRAL,
      "We will remain here with your daughter until you find an appropriate location.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme is not my daughter! Oh man! This can't get any worse!")
  TALK(SPEAKER_MEDIC, SIDE_RIGHT, EXPR_NEUTRAL,
      "Once you've found a spot for the blessed event, and signal us. We will bring her.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Ok. Got to find a pond!")
  END()

END_EVENT_SCRIPT()

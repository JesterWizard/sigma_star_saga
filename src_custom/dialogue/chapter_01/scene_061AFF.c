#include "event_macros.h"

/* Script id 129 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08061AFF. */
EVENT_SCRIPT_REPLACEMENT(0x08061AFF, scene_061AFF)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Did you know you can swap weapons while flying? Sure! Press the R Button to switch from the Custom weapon to a Standard weapon. And if you find yourself in a pinch, press the L Button to fire a Smart Bomb!")
  END()

END_EVENT_SCRIPT()

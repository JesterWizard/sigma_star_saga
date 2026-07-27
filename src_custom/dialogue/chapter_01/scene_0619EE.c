#include "event_macros.h"

/* Script id 128 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080619EE. */
EVENT_SCRIPT_REPLACEMENT(0x080619EE, scene_0619EE)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "You can combine 3 types of Gun Data to form a new weapon! Cannon Data chooses the direction of the shot. Bullet Data determines the kind of shot being released. And Impact Data tells the shot what to do when it hits a target. Check your Status Screen and get started!")
  END()

END_EVENT_SCRIPT()

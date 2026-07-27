#include "event_macros.h"

/* Script id 90 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805F58D. */
EVENT_SCRIPT_REPLACEMENT(0x0805F58D, scene_05F58D)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Hey, see that NAV COMPUTER over there?",
      "You can use it to ready a ship for takeoff.",
      "Just use the +Control Pad\nand the A Button.\nGo on, try it out."
  )
  END()

END_EVENT_SCRIPT()

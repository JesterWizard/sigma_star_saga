#include "event_macros.h"

/* Script id 98 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805FB35. */
EVENT_SCRIPT_REPLACEMENT(0x0805FB35, scene_05FB35)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Heh heh heh. Bloss sure gave you the runt of the litter!",
      "It's a good fit for you though.",
      "Next to our Krill soldiers you're pretty shrimpy."
  )
  END()

END_EVENT_SCRIPT()

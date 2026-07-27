#include "event_macros.h"

/* Script id 65 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805CF72. */
EVENT_SCRIPT_REPLACEMENT(0x0805CF72, scene_05CF72)

  TALK(SPEAKER_SOLDIER, SIDE_RIGHT, EXPR_ALT,
      "Dark empty space...sometimes it seems as though it's just out of reach...Like I could reach out and touch it.")
  END()

END_EVENT_SCRIPT()

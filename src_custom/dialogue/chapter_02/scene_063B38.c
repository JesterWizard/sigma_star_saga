#include "event_macros.h"

/* Script id 154 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08063B38. */
EVENT_SCRIPT_REPLACEMENT(0x08063B38, scene_063B38)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What's the hold up? Let's go already.")
  END()

END_EVENT_SCRIPT()

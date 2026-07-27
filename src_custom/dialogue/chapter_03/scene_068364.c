#include "event_macros.h"

/* Script id 212 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08068364. */
EVENT_SCRIPT_REPLACEMENT(0x08068364, scene_068364)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Me? Oh, I'm Recker's girlfriend. He's lost his taste for scrawny little Earth girls.")
  END()

END_EVENT_SCRIPT()

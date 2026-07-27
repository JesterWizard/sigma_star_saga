#include "event_macros.h"

/* Script id 390 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08072AB5. */
EVENT_SCRIPT_REPLACEMENT(0x08072AB5, scene_072AB5)

  TALK(SPEAKER_SLISS, SIDE_RIGHT, EXPR_NEUTRAL,
      "The legion whispers to Sliss. Angry words, get noisy! ")
  END()

END_EVENT_SCRIPT()

#include "event_macros.h"

/* Script id 356 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080709AD. */
EVENT_SCRIPT_REPLACEMENT(0x080709AD, scene_0709AD)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme, come in!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Go ahead.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I found the first probe. I hope changing the transmission frequency does the job.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "We won't know till you find the next probe. Get out of there and haul your tail to the next planet.")
  END()

END_EVENT_SCRIPT()

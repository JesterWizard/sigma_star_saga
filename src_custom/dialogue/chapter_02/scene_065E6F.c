#include "event_macros.h"

/* Script id 182 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08065E6F. */
EVENT_SCRIPT_REPLACEMENT(0x08065E6F, scene_065E6F)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "It would be my pleasure sir.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Just in case, I should go and verify that he is in his quarters.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Let us initiate a lockdown. Perhaps we'll catch the fly in our web.")
  END()

END_EVENT_SCRIPT()

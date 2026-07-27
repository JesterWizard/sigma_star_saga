#include "event_macros.h"

/* Script id 161 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08063E26. */
EVENT_SCRIPT_REPLACEMENT(0x08063E26, scene_063E26)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Excellent. Now we can triangulate the force field locations within a hair's breadth.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Done. So now what?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I've sent coordinates detailing all of the areas we need to cross to the crew.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "We just sit back and wait for the force fields to appear. Wait! What's that...?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Look out!!")
  END()

END_EVENT_SCRIPT()

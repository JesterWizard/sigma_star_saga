#include "event_macros.h"

/* Script id 162 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08063F4E. */
EVENT_SCRIPT_REPLACEMENT(0x08063F4E, scene_063F4E)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What is that thing?")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Must be an indigenous life form.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's probably attracted to the beacon's light. We'll have to scatter these aliens before the beacons can be properly activated.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I can handle it.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "If this is an infestation, your peashooter will be useless. I'm calling in a ship.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Have it summon Recker. I want to see this guy fly.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Sigh...Fine.",
      "You're up Recker. Clear a path through these bugs."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Affirmative.")
  END()

END_EVENT_SCRIPT()

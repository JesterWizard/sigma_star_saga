#include "event_macros.h"

/* Script id 327 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806E10C. */
EVENT_SCRIPT_REPLACEMENT(0x0806E10C, scene_06E10C)

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "You brought something?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yeah! Check this out! I had to fight a huge skeleton to get it!")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'm sorry, I thought you said a skeleton?",
      "You're not some fantasy nerd are you? Did you find the +1 dagger too?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Hardy har har. Look.")
  END()

END_EVENT_SCRIPT()

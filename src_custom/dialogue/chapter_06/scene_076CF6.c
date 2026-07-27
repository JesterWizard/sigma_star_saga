#include "event_macros.h"

/* Script id 443 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08076CF6. */
EVENT_SCRIPT_REPLACEMENT(0x08076CF6, scene_076CF6)

  TALK(SPEAKER_OVERLORD, SIDE_RIGHT, EXPR_NEUTRAL,
      "It is with him, the Flesh Deity who as we speak spills chaos on this planet,",
      "with him that I will merge and claim reign to the stars.",
      "In another moment, Earthman,",
      "you will be nothing more than another corpse drifting through my space.",
      "Your actions have been in vain. Already my creature is victorious,",
      "and having consumed the strength of his brethren, he now waits to feed on me.",
      "Can you feel him coming? Can you hear him calling for his master?",
      "Together we shall consume the universe!"
  )
  END()

END_EVENT_SCRIPT()

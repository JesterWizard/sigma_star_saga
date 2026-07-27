#include "event_macros.h"

/* Script id 441 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08076A4D. */
EVENT_SCRIPT_REPLACEMENT(0x08076A4D, scene_076A4D)

  TALK(SPEAKER_OVERLORD, SIDE_RIGHT, EXPR_NEUTRAL,
      "Ah, can you feel it?",
      "The pangs of birth resonate already through the first creature's deafening wail.",
      "The first planet is dying, Recker.",
      "A million forest beings eradicated as their world is crumbled about them",
      "to give birth to a new life. He comes first, the Sleeping Flesh."
  )
  END()

END_EVENT_SCRIPT()

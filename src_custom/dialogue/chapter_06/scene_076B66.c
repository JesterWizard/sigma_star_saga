#include "event_macros.h"

/* Script id 442 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08076B66. */
EVENT_SCRIPT_REPLACEMENT(0x08076B66, scene_076B66)

  TALK(SPEAKER_OVERLORD, SIDE_RIGHT, EXPR_NEUTRAL,
      "And as he spills out into the cosmos, so also emerge his five brothers.",
      "Each clawing and carving their way to the surface of their own planet,",
      "decimating eons of life and history in the process.",
      "The destruction of an entire galaxy in the course of a moment.",
      "And now, like starving dogs they tear at one another's flesh.",
      "The dance is divine, and when it is complete only one beast shall remain."
  )
  END()

END_EVENT_SCRIPT()

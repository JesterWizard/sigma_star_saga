#include "event_macros.h"

/* Script id 235 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080690B0. */
EVENT_SCRIPT_REPLACEMENT(0x080690B0, scene_0690B0)

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Now, tell him the bad news.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's good news. They are going to dust the entire site to eradicate the virus.",
      "We have to get to the hatch before this happens or we'll be atomized."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "How long do we have?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Once I get the signal, we have 5 minutes.")
  END()

END_EVENT_SCRIPT()

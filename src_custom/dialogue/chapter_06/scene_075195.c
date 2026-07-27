#include "event_macros.h"

/* Script id 427 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08075195. */
EVENT_SCRIPT_REPLACEMENT(0x08075195, scene_075195)

  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "...",
      "She'll never trust me again. Not now."
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "It was misplaced trust before.",
      "Now you can start earning the real thing."
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "She would probably still be at Starbase 5.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "'Atta boy.",
      "Grab your bug hat and let's go."
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Oh yeah. Guess I'll use it one last time.")
  END()

END_EVENT_SCRIPT()

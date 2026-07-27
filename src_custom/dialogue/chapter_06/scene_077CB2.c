#include "event_macros.h"

/* Script id 463 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08077CB2. */
EVENT_SCRIPT_REPLACEMENT(0x08077CB2, scene_077CB2)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What...",
      "Where am I?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You're with me...",
      "Safe, on Earth."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I knew somehow...",
      "You wouldn't do it..."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I had to disable your ship. I'm really sorry.",
      "But it was the only way to give you a soft landing. ",
      "I'm so sorry Psyme!"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I could...only stand there...and watch.",
      "He...destroyed everything Recker.",
      "You were right all along..."
  )
  END()

END_EVENT_SCRIPT()

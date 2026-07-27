#include "event_macros.h"

/* Script id 456 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08077276. */
EVENT_SCRIPT_REPLACEMENT(0x08077276, scene_077276)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Blune...",
      "Scarlet...",
      "Psyme...",
      "Everyone I knew and loved is dead.",
      "Was there something I could have done differently?",
      "Were we always destined to suffer and die in this war?",
      "I thought somehow, after all was said and done...",
      "That we could find a way to be happy...",
      "I was wrong.",
      "If you're listening, please...",
      "Forgive me.",
      "...",
      "Earth is safe now.",
      "This mission is over.",
      "It's time to begin a new life.",
      "A better life."
  )
  END()

END_EVENT_SCRIPT()

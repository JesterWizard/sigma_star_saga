#include "event_macros.h"

/* Script id 454 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0807716A. */
EVENT_SCRIPT_REPLACEMENT(0x0807716A, scene_07716A)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What are you doing?",
      "Use the virus!"
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "I have to go now...",
      "I should have told you."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Told me what?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Earlier, when I ran away...",
      "I gave myself the virus. It was the only way I could think of to keep it safe.",
      "I'm sorry."
  )
  END()

END_EVENT_SCRIPT()

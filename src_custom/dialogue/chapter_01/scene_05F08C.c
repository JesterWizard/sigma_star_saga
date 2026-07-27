#include "event_macros.h"

/* Script id 87 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0805F08C. */
EVENT_SCRIPT_REPLACEMENT(0x0805F08C, scene_05F08C)

  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Hear me in your ear?",
      "We implanted a chip behind your ear. I can send you a one-way signal."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Thanks for consulting me first.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Ha!! Use that anger; it will help sell your story.",
      "I'll be in contact again soon."
  )
  END()

END_EVENT_SCRIPT()

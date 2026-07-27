#include "event_macros.h"

/* Script id 264 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806A353. */
EVENT_SCRIPT_REPLACEMENT(0x0806A353, scene_06A353)

  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "Long time, kid! Commander Zelly said you're looking for an AWOL pilot.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "So I can execute him. Is this really standard Krill procedure? Eliminating an entire Starbase crew?")
  TALK(SPEAKER_BLOSS, SIDE_RIGHT, EXPR_NEUTRAL,
      "You mean she really killed them all? Phooo...Foul woman that one.",
      "Worse than the Tyrannical Overlord I'd bet."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Well Blune's blood will be on her hands. Where is he?")
  END()

END_EVENT_SCRIPT()

#include "event_macros.h"

/* Script id 285 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806B742. */
EVENT_SCRIPT_REPLACEMENT(0x0806B742, scene_06B742)

  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Excellent. I will begin analysis at once.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "As I mentioned earlier, the girl Scarlet may be helpful on this.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Give it a rest, Recker!")
  END()

END_EVENT_SCRIPT()

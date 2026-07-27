#include "event_macros.h"

/* Script id 216 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080688A6. */
EVENT_SCRIPT_REPLACEMENT(0x080688A6, scene_0688A6)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Well, that's all I need. Recker?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Hold on. You can't kill her!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Why not? We can't leave her here to breed a virus that will destroy the Krill!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Then she's coming with us. We'll have better chances of surviving if we stick together.")
  END()

END_EVENT_SCRIPT()

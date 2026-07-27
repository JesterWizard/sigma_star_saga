#include "event_macros.h"

/* Script id 242 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806930F. */
EVENT_SCRIPT_REPLACEMENT(0x0806930F, scene_06930F)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Hey!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Cool off Recker. This is Scarlet. She's a gift for Commander Nomak, so hands to yourself!")
  TALK(SPEAKER_EK, SIDE_RIGHT, EXPR_NEUTRAL,
      "That's a crying shame, that is! I lost my pet a week ago today! Could do with a replacement.")
  END()

END_EVENT_SCRIPT()

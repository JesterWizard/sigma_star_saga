#include "event_macros.h"

/* Script id 280 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806B604. */
EVENT_SCRIPT_REPLACEMENT(0x0806B604, scene_06B604)

  TALK(SPEAKER_EK, SIDE_RIGHT, EXPR_ALT,
      "That Scarlet lass had better get real friendly real quick! If we can't tame  her, we'll put 'er down.")
  END()

END_EVENT_SCRIPT()

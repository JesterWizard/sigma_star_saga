#include "event_macros.h"

/* Script id 384 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08072490. */
EVENT_SCRIPT_REPLACEMENT(0x08072490, scene_072490)

  TALK(SPEAKER_CREWMAN, SIDE_RIGHT, EXPR_ALT,
      "Commander! Your room is all ready.")
  END()

END_EVENT_SCRIPT()

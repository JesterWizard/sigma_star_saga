#include "event_macros.h"

/* Script id 116 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08061298. */
EVENT_SCRIPT_REPLACEMENT(0x08061298, scene_061298)
  TEXT("YOU RECEIVED THE SCANNING TOOL!")
END_EVENT_SCRIPT()

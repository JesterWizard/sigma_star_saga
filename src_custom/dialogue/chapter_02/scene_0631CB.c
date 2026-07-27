#include "event_macros.h"

/* Script id 149 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080631CB. */
EVENT_SCRIPT_REPLACEMENT(0x080631CB, scene_0631CB)

  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_ALT,
      "You transferred here with Psyme? Man, what a hothead.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'll say. Is she always like that?")
  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_NEUTRAL,
      "Look, I'm supposed to clean the COMM ROOM. If you do it for me, I'll give you all the dirt on Psyme. Whadaya say?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'll need an access key.")
  TALK(SPEAKER_TECH, SIDE_RIGHT, EXPR_NEUTRAL,
      "That's no problem. I have an extra.")
  END()

END_EVENT_SCRIPT()

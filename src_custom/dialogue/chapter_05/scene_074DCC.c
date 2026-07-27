#include "event_macros.h"

/* Script id 417 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08074DCC. */
EVENT_SCRIPT_REPLACEMENT(0x08074DCC, scene_074DCC)

  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "This is truly Earth's finest hour. ",
      "The hatching can not be stopped.",
      "The Krill are defeated.",
      "And now!",
      "I am the most powerful man",
      "On EARTH!!",
      "Ho!",
      "Ha ha ha!",
      "HAAAAH  HA HA HA HAHHH!!"
  )
  END()

END_EVENT_SCRIPT()

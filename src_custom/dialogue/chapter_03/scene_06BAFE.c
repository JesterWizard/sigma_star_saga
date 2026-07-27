#include "event_macros.h"

/* Script id 289 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806BAFE. */
EVENT_SCRIPT_REPLACEMENT(0x0806BAFE, scene_06BAFE)

  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker! Report ASAP!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Roger that! Oh, I forgot it's a one way signal.",
      "I'll have to risk the COMM ROOM again."
  )
  END()

END_EVENT_SCRIPT()

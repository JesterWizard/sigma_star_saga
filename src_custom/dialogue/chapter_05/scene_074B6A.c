#include "event_macros.h"

/* Script id 414 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08074B6A. */
EVENT_SCRIPT_REPLACEMENT(0x08074B6A, scene_074B6A)

  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's done.",
      "All traces of the virus are gone."
  )
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Well done. Your payment will be forthcoming.")
  END()

END_EVENT_SCRIPT()

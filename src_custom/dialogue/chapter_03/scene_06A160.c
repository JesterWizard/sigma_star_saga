#include "event_macros.h"

/* Script id 263 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806A160. */
EVENT_SCRIPT_REPLACEMENT(0x0806A160, scene_06A160)

  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "That way High Command will have only you, Psyme, and myself to blame if there are further holes in Krill security.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You want me to kill Blune?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "It will be a simple matter. He took a shuttle to Starbase 1, faking a transfer to cover his tracks.",
      "He won't see it coming. Not from you."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "And what's to keep you from bumping me off next?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "I have shielded you from certain details. Rest assured, Mister Recker, you are no great threat to me.",
      "Get it done and report back to me.",
      "Dismissed."
  )
  END()

END_EVENT_SCRIPT()

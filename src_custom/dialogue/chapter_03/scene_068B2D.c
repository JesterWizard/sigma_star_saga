#include "event_macros.h"

/* Script id 218 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08068B2D. */
EVENT_SCRIPT_REPLACEMENT(0x08068B2D, scene_068B2D)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Scarlet, are those flares?",
      "On your belt?"
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Oh, these things? The light will only rile up those dead humans though. It's the first thing I tried.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "But now there are three of us, plus we know a station is up there expecting us.",
      "I could plant the flares in a wide formation while you two cover my back.",
      "If they're still scanning up there, we might be able to catch their eye."
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'd guess TEN FLARES ought to do it. Find dry spots and jam them pointy side down.")
  END()

END_EVENT_SCRIPT()

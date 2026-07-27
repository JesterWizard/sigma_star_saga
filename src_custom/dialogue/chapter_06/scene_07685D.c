#include "event_macros.h"

/* Script id 440 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0807685D. */
EVENT_SCRIPT_REPLACEMENT(0x0807685D, scene_07685D)

  TALK(SPEAKER_OVERLORD, SIDE_RIGHT, EXPR_NEUTRAL,
      "I am surprised you were man enough to destroy her Mister Recker.",
      "I see that you, like me, would sacrifice your own to see your goals achieved.",
      "Exactly what I was hoping for.",
      "I will enjoy keeping your company on this blessed occasion.",
      "This life is too short for planetary allegiance, soldier.",
      "Soon the galaxy will be decimated, and once I have usurped its power,",
      "I will unleash a wave of destruction beginning with your blue, gutted planet.",
      "Everyone you've ever known, burned from existence."
  )
  END()

END_EVENT_SCRIPT()

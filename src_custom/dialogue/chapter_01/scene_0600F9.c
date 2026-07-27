#include "event_macros.h"

/* Script id 104 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x080600F9. */
EVENT_SCRIPT_REPLACEMENT(0x080600F9, scene_0600F9)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "That blast was the Shell fleet deploying.",
      "By now you must already know that our ships are living things.",
      "Standard operating procedure any time we land a starbase is to set the fleet loose to patrol the surrounding airspace.",
      "They make good eyes and ears, but our ships aren't capable of evasive maneuvers.",
      "When a ship is in danger, it summons the nearest pilot into its Control Cavity.",
      "The pilot acts as its logic center until the threat is eliminated.",
      "The beauty of this design is that we can breed our ships to outnumber pilots 100 to 1. It's been the key to our success in dominating this sector of space."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "When you said the ship \"summons the pilot\"...",
      "You mean it...",
      "Calls you or something?"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "You don't know?")
  END()

END_EVENT_SCRIPT()

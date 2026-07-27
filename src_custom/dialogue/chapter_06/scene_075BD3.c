#include "event_macros.h"

/* Script id 433 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08075BD3. */
EVENT_SCRIPT_REPLACEMENT(0x08075BD3, scene_075BD3)

  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker! What happened?",
      "Was Psyme here?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "...",
      "She's gone."
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "We can't stay. Not in orbit over the Krill Homeworld while that monstrous thing wakes up. We've got to get away.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "...",
      "I can't leave her.",
      "...",
      "Not again.",
      "...",
      "Will the ships on this starbase still link with my parasite?"
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "You're asking me?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "...",
      "...",
      "...",
      "I'm going to the surface.",
      "I'm gonna get her back. She's all that matters to me now."
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Ok, but hurry! I don't want to be anywhere near here when that planet blows!")
  END()

END_EVENT_SCRIPT()

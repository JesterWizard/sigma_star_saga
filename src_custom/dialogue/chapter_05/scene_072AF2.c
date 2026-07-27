#include "event_macros.h"

/* Script id 391 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x08072AF2. */
EVENT_SCRIPT_REPLACEMENT(0x08072AF2, scene_072AF2)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Now don't tell me THAT's normal.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Who knows with them. Guess it's your turn to baby sit.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "They are really creepy. Will you be OK?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I don't believe in ghosts. ")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'll scatter the ships just in case. ")
  END()

END_EVENT_SCRIPT()

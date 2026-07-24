#include "dialogue_macros.h"

/* ROM 0x0806A58D */
DIALOGUE_SCRIPT(0x0806A58D, scene_06A58D)
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Hello Recker. Thought I'd never see you again.",
      "Psyme...?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "She's fine. Zelly tried to make it look like an accident. She dumped us on a virus infested Planet, but her plan failed.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Glad to hear it. Something huge is going down. High Command ordered Commander Zelly to slay her crew once they analyzed the scan data of the first three planets.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "She said the crew was killed because of a security breach.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "That's just a cover story.",
      "High Command wants to be the only ones who know which planets are synchronizing.",
      "Once they've found them all, you and Psyme will be eliminated."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Blune...I'm here because Zelly sent me.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "I figured as much. Were you instructed to bring me in alive?")
END_DIALOGUE_SCRIPT()

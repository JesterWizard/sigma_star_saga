#include "dialogue_macros.h"

/* ROM 0x08069EEA */
DIALOGUE_SCRIPT(0x08069EEA, scene_069EEA)
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Welcome back Mister Recker. I trust you and your partner arrived safely at Starbase 3?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "There was a slight detour, but nothing we couldn't handle.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "I've been doing some spring cleaning as you can see.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yeah, I see it.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "High Command was very disappointed to learn of a spy within our ranks. It was necessary to alleviate all doubt.",
      "Only one task remains.",
      "I've just spoken with your new commander, Nomak. He's allowed me to enlist your services one last time."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What do you have in mind?")
END_DIALOGUE_SCRIPT()

#include "dialogue_macros.h"

/* ROM 0x0806CC21 */
DIALOGUE_SCRIPT(0x0806CC21, scene_06CC21)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Ek squealed like a girl. And you will too Nomak.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme, what are you doing?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "That Attack was too close!",
      "I'm through taking orders from you imbeciles!",
      "Tell me everything you got from that black box!",
      "Do it now or you're history!"
  )
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Don't do anything crazy! What are you turning on me for?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's the High Command that's turned on all of us!",
      "We're doing their dirty work and they're slaughtering us left and right!",
      "For the last time, what is on that black box?!"
  )
  TALK(SPEAKER_NOMAK, SIDE_RIGHT, EXPR_NEUTRAL,
      "Nothing! It was empty! Just these Sand Planet coordinates, I swear!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "That's all we need. Let's go!")
END_DIALOGUE_SCRIPT()

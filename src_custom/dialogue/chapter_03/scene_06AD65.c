#include "dialogue_macros.h"

/* ROM 0x0806AD65 */
DIALOGUE_SCRIPT(0x0806AD65, scene_06AD65)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Blune!")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Gasp...Gasp...",
      "Take...it..."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What have you done?!")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "My parasite's tail...",
      "It will live...",
      "But it had a good grip...",
      "On my spine..."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "This looks bad...",
      "Will you be ok?"
  )
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "My chances are pretty good. The parasite will try to regenerate. Leave me here.",
      "But Recker...",
      "We can't follow Earth or Krill blindly anymore.",
      "Good people are being slaughtered on both sides to protect this weapon."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I know.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "On both sides.",
      "Think about what that means before you take action."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I will.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'll find you later. Don't worry about me.",
      "Go back to Zelly while that tail's still warm."
  )
END_DIALOGUE_SCRIPT()

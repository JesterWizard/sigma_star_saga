#include "dialogue_macros.h"

/* ROM 0x0806845E */
DIALOGUE_SCRIPT(0x0806845E, scene_06845E)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "-under attack by a horde of diseased humans. They looked crazed somehow. Like, well...")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Like B-movie zombies.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Are there more humans here? Like you?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Nope, nuh-uh. I've ID'd all of the scientists and support team. I'm the only uninfected person left.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "How did this happen?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "We were sent here by AEF to breed a very specific strain of virus.",
      "We were successful, but a byproduct of the virus contaminated our food stores somehow. By morning everyone was infected.",
      "I was watching my carbs, so I sort of dodged that bullet.",
      "Girl's gotta watch her figure, right purple?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You said you were breeding a virus. What for?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "We were given a sample of Krill genetic material. The virus was required to multiply and destroy the material utterly.",
      "It was quite a challenge, since the sample was completely foreign to us, but we..."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Stop. Am I at risk?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "No, I doubt it.",
      "The virus running amok here is not the pure strain. Like I said, just a byproduct.",
      "The real virus only exists in code until we manufacture it.",
      "I still remember the formula...Maybe I could whip some up for you?"
  )
END_DIALOGUE_SCRIPT()

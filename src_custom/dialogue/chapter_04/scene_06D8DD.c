#include "dialogue_macros.h"

/* ROM 0x0806D8DD */
DIALOGUE_SCRIPT(0x0806D8DD, scene_06D8DD)
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "I've been given instructions. High Command accepts your offer.",
      "You are free to use this facility to create an anti-virus on the condition that you turn it in upon completion.",
      "Do this and I will guarantee your freedom."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Good.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Here is a schematic of the virus. We will need to create both the virus, and the anti-virus to be certain it all works.")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "Let's take a look. Hmmm.",
      "...",
      "We will need to create a Genome to my specifications.",
      "But certain base genetic elements will be required."
  )
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "I would assume some fossil record exists on the planet below.")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "It would take only a single preserved specimen to extract the genetic code we need.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "Looks like we'll have to scavenge for genetic building blocks.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Psyme, we'll need someone to look after our interests here. We can't allow ourselves to get backed into a corner again.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "You'd better keep your hands to yourself.")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "We'll see!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I hope you get a sunburn, pink skin!")
END_DIALOGUE_SCRIPT()

#include "dialogue_macros.h"

/* ROM 0x08065BC4 */
DIALOGUE_SCRIPT(0x08065BC4, scene_065BC4)
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "What? Psyme! Why did you not come to me with this information?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "There have been long range transmissions from Starbase 2 that do not coincide with transmissions from the other stations, or High Command. The fact that Earth had a mining operation in place suggests that they have been eavesdropping on our plans.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "What about the human pilot? Surely he's the leak.")
  TALK(SPEAKER_OVERLORD_SHADOW, SIDE_RIGHT, EXPR_NEUTRAL,
      "I have verified his story through my own means. And he entered into our service only recently, which does not explain the presence of humans on the Forest Planet. But as a precaution, once I know the location of all six synchronized planets we will dispose of him.",
      "That will be your responsibility Psyme."
  )
END_DIALOGUE_SCRIPT()

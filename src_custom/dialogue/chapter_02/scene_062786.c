#include "dialogue_macros.h"

/* ROM 0x08062786 */
DIALOGUE_SCRIPT(0x08062786, scene_062786)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Recker reporting for duty!")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Welcome to Starbase 2, Mister Recker.",
      "You will address me as Commander Zelly.",
      "You will find that I run a very tight ship, unlike your previous Commander's putrid animal stable.",
      "He briefed you on the purpose of your transfer?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yes ma'am. He suggested that High Command was pleased with my piloting skills and saw fit to promote me.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "A transfer and a promotion are two very different things Mister Recker.",
      "You should know that I read all about how you came to be in Bloss's service.",
      "A crack pilot found clinging to life in a human prison ship.",
      "Rather unconventional way to join our ranks, wouldn't you say?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yes ma'am, I would.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "And here you stand, adorned in the armor of a Krill, wandering among us? Can I rely upon you to follow my orders?",
      "Even those that may turn you against your own kind?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Maybe you missed the footnote in Bloss's report.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Oh, about the human mining facility? Yes, I read it.",
      "But don't pretend one skirmish proves your allegiance to Krill Command...I still find it suspicious. "
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "If you don't trust me yourself, what is your purpose in bringing me here?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Oh, I didn't bring you here dear boy. The order came from the Tyrannical Overlord.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "The Tyrannical Overlord?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Our leader. The head of the Krill High Command.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "He's in charge? Why the interest in me?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "That information is beyond me. But I do know that while you're here, you'll",
      "follow my orders to the letter. Clear?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Very.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Good. We are to continue the work begun on the Forest Planet by Commander Bloss.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Another geological survey?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Blune will fill you in.")
END_DIALOGUE_SCRIPT()

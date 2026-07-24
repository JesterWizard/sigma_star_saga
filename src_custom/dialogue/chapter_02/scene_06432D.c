#include "dialogue_macros.h"

/* ROM 0x0806432D */
DIALOGUE_SCRIPT(0x0806432D, scene_06432D)
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "What did you find?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Does this fissure look deep enough?")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "We'll know in a minute.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Why do you suppose Krill High Command is so interested in this planet?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's hard to say. High Command has conducted more of these kinds of scanning missions in the past few years than in our people's entire history. Since the original excavation on Earth it's been the Tyrannical Overlord's favorite pastime.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You said excavation of Earth?! Are you referring to the first attack on Earth that started the war?")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Let's get something straight Earthman. The Krill are not at war with you.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Not at war? What do you call the last 60 years?!",
      "Psyme, you yourself led an attack just last month! How can you say you're not at war?"
  )
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "No one is trying to conquer your planet Recker. The excavation of Earth was deemed necessary by Krill High Command. Your people responded in violence.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You nearly destroyed all life on Earth! It caused cataclysmic global damage!",
      "Don't you see that your actions have consequences?",
      "You have created an enemy of billions of people!"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "That's their mistake. They can't stand against the Krill. Our armada is endless! We can stamp them out effortlessly whenever we choose!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yeah?! Well you did a pretty sloppy job of wiping us out last month, didn't you?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "For your information, that was just a follow up visit to assess the damage to your planet! Your idiot pilots forced the battle!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I refuse to believe you planet butchers were making a house call!!")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Give it a rest both of you. Understand Recker. Your people are like frightened animals. We drew a little blood, and they bit us.",
      "It's as simple as that."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Are you actually saying that the attack on Earth was for our own good?")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'm saying that with time you will see that things aren't so black and white. For now, let's see what the scans turned up.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "I've got it.",
      "...",
      "...",
      "This is impossible."
  )
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "What?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "These readings. They're the same.",
      "Recker, you must not have cleared out the old data from the Forest Planet scans."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Don't blame me! We're not even using the same equipment!")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "But then...",
      "How is it that both the Forest Planet and this planet are experiencing tremors that display exactly the same pattern?"
  )
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "It's a malfunction. Two planets can't exhibit this level of synchronicity. There must be another answer.",
      "Psyme, see if there's a larger entrance to this fissure. One we could take a ship into."
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "These readings suggest several openings. But it would be a hazardous journey.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "We need more information. Recker, can you navigate small spaces?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You know I can. Call a ship.")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'll ride shotgun. We'll proceed to the source of the tremors and see if we can't get to the bottom of things.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Here's a DATA PLUG. You'd better record your progress first.")
END_DIALOGUE_SCRIPT()

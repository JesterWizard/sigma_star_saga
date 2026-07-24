#include "dialogue_macros.h"

/* ROM 0x0806D132 */
DIALOGUE_SCRIPT(0x0806D132, scene_06D132)
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "I am Professor Zart. Welcome to Starbase 4, or as we call it here, Zart Laboratories.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Hah! Did you name it yourself?")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "Show respect! Don't you realize I am science advisor to the Tyrannical Overlord?",
      "Why would you jump ship and abandon Commander Nomak like that?",
      "Surely you knew you would be placed under house arrest upon arrival?"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Naturally. But we also knew that Nomak would inform you that the virus is harmless to Krill physiology. And that your Starbase would be moved here to accommodate our visit.")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "True! High Command was very pleased to learn that this dustball planet was the fourth! But again, you are no fool. Why risk coming here?")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Because the situation has become volatile.",
      "We need to straighten some things out with his majesty.",
      "You are our best chance of getting his ear.",
      "We want our freedom. All three of us."
  )
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "What do you offer in return?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Information about the virus.")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "That is hardly worth your lives! The virus is no longer a threat.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Not to the people of Krill. But unless I'm wrong it will be quite effective against the Tyrannical Overlord's precious alien matter!")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "My word!!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Even the human scientists never knew what they were creating. It was Earth's failsafe plan in case your overlord got the alien matter first.")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "Where is this virus now?")
  TALK(SPEAKER_SCARLET, SIDE_RIGHT, EXPR_NEUTRAL,
      "In a safe place, along with a few samples that were transferred to Earth. Let us go free, and I will dictate the makeup of the virus to you exactly.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "You can use the formula to create an anti-virus. If Earth deploys it against his Excellency's alien matter, an anti-virus will be his only defense.")
  TALK(SPEAKER_ZART, SIDE_RIGHT, EXPR_NEUTRAL,
      "You were indeed wise to come directly to me. I will contact High Command and the Tyrannical Overlord.")
END_DIALOGUE_SCRIPT()

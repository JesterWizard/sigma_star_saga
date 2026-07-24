#include "dialogue_macros.h"

/* ROM 0x0806BB74 */
DIALOGUE_SCRIPT(0x0806BB74, scene_06BB74)
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Tierney here.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "All right Commander. I've got details, but this time I want answers.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "I called you, but fair enough.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "When you sent me here, how many planets did you know were synchronized?")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Allied Earth Federation knew about two of them. A third was discovered shortly after, but the report never reached us. It's probably still documented at the lab.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I've removed all critical intel from that lab, sir, so unless I'm mistaken I'm the only one with the coordinates for the fourth planet.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "What do you want?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Tell me how many planets there are.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Blackmail, Recker? Really...",
      "I suppose I've no choice but to compromise Earth security. Is that what you want?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'm waiting.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "There are six planets, all synchronized. We have three of them ID'd.",
      "What was the status of the laboratory?"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "All dead but for one survivor. Scarlet Keys.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "The virus she was creating is critical to our success. Is it safe?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "As long as she is. They never completed a working version. It's still in her head.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Until we have the ID of all six planets, Miss Keys' safety is priority one. Now transmit the coordinates of the fourth planet.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I don't have them.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "What?! But you said-")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I have the black box. It will need to be analyzed before I can identify the next planet.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Now if you're through making ridiculous demands, I have an urgent matter that concerns us both.",
      "An attack on your Starbase is imminent."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What? Why didn't you say so!")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "The Earth armada is here to recover the virus and the black box.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Good. Then we can end all this craziness.")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "You have to repel the attack.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "What?! Attack the Earth fleet? Why?!")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "We need the Krill to lead us to the last two planets! If Earth takes the black box now, we're back to square one! The only future for humanity is either to use the virus to destroy the Krill homeworld, or find the Krill weapon before they do.",
      "And for that we need you, Miss Keys, and the black box intact.",
      "Attacking our own fleet is in Earth's best interest! You must use every resource at your disposal!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "How can you say that?! Call them off!!")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "We're operating in the dark! I don't have that kind of authority!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'm done killing Earth's soldiers, sir!")
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Billions more will die if you stand there and do nothing!!",
      "You must attack!!",
      "Attack now, or their blood will be on your hands!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "At least let me transfer Scarlet! DON'T MAKE ME DO THIS!!",
      "TIERNEY?! TALK TO ME!!"
  )
END_DIALOGUE_SCRIPT()

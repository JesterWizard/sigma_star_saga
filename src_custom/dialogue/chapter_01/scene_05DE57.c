#include "dialogue_macros.h"

/* ROM 0x0805DE57 */
DIALOGUE_SCRIPT(0x0805DE57, scene_05DE57)
  TALK(SPEAKER_MEDIC, SIDE_RIGHT, EXPR_NEUTRAL,
      "Ahh, you're coming around.",
      "Try not to strain your muscles."
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "Nngh....")
  TALK(SPEAKER_MEDIC, SIDE_RIGHT, EXPR_NEUTRAL,
      "Do you know where you are? Can you recognize me?")
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "You...",
      "...you are...",
      "...a Krill..."
  )
  TALK(SPEAKER_MEDIC, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'll admit I'm surprised. You should be quite dead by now. You are on a Krill starbase. A fairly run down one, but a starbase nonetheless.")
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "...how...")
  TALK(SPEAKER_MEDIC, SIDE_RIGHT, EXPR_NEUTRAL,
      "Your security shuttle had the misfortune of crossing paths with one of our warships.",
      "We found your body while salvaging the wreckage. I'd daresay Earth was glad to be rid of you, judging by the maximum security cell!",
      "It must have been expensive to banish you to space. You must have quite a history. "
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "I remember...",
      "...it was dark...stale...",
      "How long...",
      "...have I been asleep?"
  )
  TALK(SPEAKER_MEDIC, SIDE_RIGHT, EXPR_NEUTRAL,
      "Well now, I wouldn't call it sleep.",
      "You were delirious for the first eight days and unconscious for nearly three weeks after that."
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "...a month...",
      "...it's all so confusing...",
      "...hard to remember...",
      "Why...",
      "Why are you helping me?"
  )
  TALK(SPEAKER_MEDIC, SIDE_RIGHT, EXPR_NEUTRAL,
      "I'm sorry to say that restoring you to a functional state is no rescue.",
      "Commander Bloss has plans for you.",
      "Unless I'm mistaken, he plans to meet you in the ring."
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "He wants...to fight me?")
  TALK(SPEAKER_MEDIC, SIDE_RIGHT, EXPR_NEUTRAL,
      "Bloss is, or was in his youth, a fighting champion.",
      "On this starbase, it's the only Krill pastime we have.",
      "The crew is very excited to see their first Earthman fight."
  )
  TALK(SPEAKER_RECKER, SIDE_LEFT, EXPR_NEUTRAL,
      "We're fighting...",
      "...to the death?"
  )
  TALK(SPEAKER_MEDIC, SIDE_RIGHT, EXPR_NEUTRAL,
      "Well, that all depends.",
      "I think I can buy you a bit more time...",
      "Try to rest.",
      "They'll be skinning you in the morning."
  )
END_DIALOGUE_SCRIPT()

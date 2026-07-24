#include "dialogue_macros.h"

/* ROM 0x0805DB84 */
DIALOGUE_SCRIPT(0x0805DB84, scene_05DB84)
  TALK(SPEAKER_MEDIC_1, SIDE_LEFT, EXPR_NEUTRAL,
      "...badly...",
      "...extensive damage..."
  )
  TALK(SPEAKER_MEDIC_2, SIDE_RIGHT, EXPR_NEUTRAL,
      "...try to...",
      "...no use..."
  )
  TALK(SPEAKER_MEDIC_1, SIDE_LEFT, EXPR_NEUTRAL,
      "Bah, look at what those grunts did. There's hardly anything left to work with.")
  TALK(SPEAKER_MEDIC_2, SIDE_RIGHT, EXPR_NEUTRAL,
      "This guy's been through plenty all right. It's a wonder he's still alive at all.")
  TALK(SPEAKER_MEDIC_3, SIDE_LEFT, EXPR_NEUTRAL,
      "He can't croak soon enough for me! Commander Bloss has signed the carcass over to my office!")
  TALK(SPEAKER_MEDIC_1, SIDE_LEFT, EXPR_NEUTRAL,
      "Feh...If you need an Earthman sample so badly, snap off a finger. He won't need 'em where he's going.")
  TALK(SPEAKER_MEDIC_2, SIDE_RIGHT, EXPR_NEUTRAL,
      "Would you two back off? I need room to operate.")
  TALK(SPEAKER_MEDIC_1, SIDE_LEFT, EXPR_NEUTRAL,
      "Waste of time. This fellow's at Death's door. No way can he fight in this condition.")
  TALK(SPEAKER_MEDIC_3, SIDE_LEFT, EXPR_NEUTRAL,
      "Then he's mine either way. He won't last five...",
      "...in the..."
  )
  TALK(SPEAKER_MEDIC_1, SIDE_LEFT, EXPR_NEUTRAL,
      "...slipping...",
      "...hand me...",
      "...still...to save..."
  )
  TALK(SPEAKER_MEDIC_2, SIDE_RIGHT, EXPR_NEUTRAL,
      "...too late...")
END_DIALOGUE_SCRIPT()

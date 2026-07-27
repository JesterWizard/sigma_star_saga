#include "event_macros.h"

/* Script id 179 — map/NPC talk via event records (@ 0x080179C8 → StartTalkById). No cutscene FSM. Talk ROM 0x0806596F. */
EVENT_SCRIPT_REPLACEMENT(0x0806596F, scene_06596F)

  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Your Excellency, the patterns we scanned on the Fire Planet match the ones you directed us to on the Forest Planet.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "The similarities are frightening.")
  TALK(SPEAKER_OVERLORD_SHADOW, SIDE_RIGHT, EXPR_NEUTRAL,
      "Then we have found two of the six remaining specimens.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "There are six planets with identical quake patterns?")
  TALK(SPEAKER_OVERLORD_SHADOW, SIDE_RIGHT, EXPR_NEUTRAL,
      "Yes. And I am disgusted to hear that the humans were already exploiting the Forest Planet when our forces arrived. Such failure is unforgivable.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Then you will deal with Commander Bloss?")
  TALK(SPEAKER_OVERLORD_SHADOW, SIDE_RIGHT, EXPR_NEUTRAL,
      "In due time. First I have a more pressing matter.")
  END()

END_EVENT_SCRIPT()

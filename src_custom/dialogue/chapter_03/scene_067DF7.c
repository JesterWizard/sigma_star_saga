#include "event_macros.h"

/*
 * Script id 205 — cutscene-fired talk.
 *
 * FSM StartTalkById @ 0x08049F8C.
 * Talk ROM 0x08067DF7.
 *
 * No invented SPAWN/WALK — only macros evidenced by the owner FSM.
 * compile_dialogue.py packs TALK/TEXT; compile_events.py may emit a runner table.
 */

EVENT_SCRIPT_REPLACEMENT(0x08067DF7, scene_067DF7)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I want to know what this is all about, and you should too!",
      "This alien matter could be a weapon!"
  )
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Who knows? It could just as easily be starship food!",
      "You know, your meddling is getting out of hand, and I'm going to be the one who gets bit on the backside!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'm going to look around inside the compound.")
  TALK(SPEAKER_PSYME, SIDE_RIGHT, EXPR_NEUTRAL,
      "Go! I've had it up to here with you!")
  END()

END_EVENT_SCRIPT()

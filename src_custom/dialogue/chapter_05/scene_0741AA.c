#include "event_macros.h"

/*
 * Script id 409 — cutscene-fired talk.
 *
 * FSM StartTalkById @ 0x08049E3E.
 * Talk ROM 0x080741AA.
 *
 * No invented SPAWN/WALK — only macros evidenced by the owner FSM.
 * compile_dialogue.py packs TALK/TEXT; compile_events.py may emit a runner table.
 */

EVENT_SCRIPT_REPLACEMENT(0x080741AA, scene_0741AA)

  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Blune! Turn back!")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "Don't follow me Recker!",
      "I need to upload the anti-virus to Tierney!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Blune!",
      "The Krill planet will be destroyed!",
      "The war will continue...",
      "Worse than before! "
  )
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "How can I trust you? ",
      "You're the leader of Sigma Team!",
      "Tierney hired you to seize the weapon!"
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "By giving Earth the anti-virus you're condemning your own planet!")
  TALK(SPEAKER_BLUNE, SIDE_RIGHT, EXPR_NEUTRAL,
      "I wish I could believe you Recker.",
      "But you've told one lie too many.",
      "I have to upload the anti-virus.",
      "It will all be over in 30 seconds."
  )
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker! Are you mad? Call it off!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Upload the virus to me Commander!",
      "There is still a chance to stop the bio weapon!"
  )
  TALK(SPEAKER_TIERNEY, SIDE_RIGHT, EXPR_NEUTRAL,
      "I won't do that Recker! Hold your fire!")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "No!")
  END()

END_EVENT_SCRIPT()

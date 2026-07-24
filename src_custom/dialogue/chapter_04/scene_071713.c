#include "dialogue_macros.h"

/* ROM 0x08071713 */
DIALOGUE_SCRIPT(0x08071713, scene_071713)
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Looks like his schedule's clear now.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Recker, why did you come back?")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "I'm attempting to sabotage the Overlord's bio weapon. He's been using all of us to search for it.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "I knew there were six special planets. But I don't know why he needed them.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Well, Psyme and I found one he's missing. It's a Forgotten Planet...no starbase there. Without one, we're helpless.")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "You need me to transfer a station there.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Can you do it without the Overlord finding out?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Effortlessly.",
      "Starbase 5 will find new orders, placing it above the Forgotten Planet within a few hours. The crew will follow commands upon your arrival."
  )
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "How can I make it up to you?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "You already have. But Recker, do one thing more.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Yes?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Take out the man at the top.")
  TALK(SPEAKER_RECKER_KRILL, SIDE_LEFT, EXPR_NEUTRAL,
      "Will you be safe here?")
  TALK(SPEAKER_ZELLY, SIDE_RIGHT, EXPR_NEUTRAL,
      "Yes, it will take a while for High Command to catch on. Congratulations Mister Recker, you've just hijacked a Krill Starbase.")
END_DIALOGUE_SCRIPT()

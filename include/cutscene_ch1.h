#ifndef GUARD_CUTSCENE_CH1_H
#define GUARD_CUTSCENE_CH1_H

#include "global.h"

/* Vanilla @ 0x080523EC — Chapter 1 opening cutscene per-frame FSM.
 * Step 10 calls StartTalkById(51) → scene_05C9E1. */
void CutsceneCh1Opener(void);

/* Vanilla label @ 0x0802BFA0 — JT case 0 inside stage FSM @ 0x0802B18C.
 * Starts StartTalkById(109) → scene_0605BB (Target Counter briefing).
 * Append reference peel; parent not LynJumped yet. */
void CutsceneCh1TargetBrief(void);

#endif /* GUARD_CUTSCENE_CH1_H */

#ifndef GUARD_CUTSCENE_STAGE_H
#define GUARD_CUTSCENE_STAGE_H

#include "global.h"

/* Vanilla @ 0x0802B18C — per-frame stage / cutscene dispatcher.
 * Outer index: gStageCase (0..0x10D). Talks ids 109 (case 0) and 451
 * (case 175). Case 10 calls CutsceneCh1Opener.
 *
 * Append reference peel; LynJump when .custom_cutscene_stage is TRUE. */
void CutsceneStageUpdate(void);

#endif /* GUARD_CUTSCENE_STAGE_H */

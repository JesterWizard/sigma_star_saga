#ifndef GUARD_DEBUG_TEXT_H
#define GUARD_DEBUG_TEXT_H

#include "gba/types.h"

/*
 * Soft debug text (baserom).
 *
 * ClearSoftTextMap @ 0x08009A90 — CpuSet-fill gSoftTextMap, set dirty bit 8
 * DrawDebugText    @ 0x08009480 — glyph → halfword entries in gSoftTextMap
 *
 * Soft map is flushed to a BG screenbase when dirty bit 8 is set (VBlank path).
 * While the debug menu owns BG0 it copies the soft map itself and clears the bit.
 */

#define SOFT_TEXT_ENTRIES 0x2C0 /* 32×22 cells */
#define SOFT_TEXT_DIRTY_BIT 8

extern u16 gSoftTextMap[]; /* 0x030050F0 */
extern u8 gSoftTextDirty;  /* 0x03000C9C — bit 8 = text map dirty */

void ClearSoftTextMap(void);
void DrawDebugText(const char *str, u32 x, u32 y, u32 palBank, u32 a, u32 b);

#endif /* GUARD_DEBUG_TEXT_H */

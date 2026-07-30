#include "global.h"
#include "debug_text.h"
#include "ram_map.h"

/*
 * ClearSoftTextMap / DrawDebugText — decompiled from baserom.
 *
 * Linked into append ROM (not peeled): callers use the baserom copies via
 * Thumb addresses so we cannot drift from vanilla text behaviour. This C is
 * the readable reference for the soft-map / dirty-flag contract.
 */

typedef void (*CpuSetFn)(const void *src, void *dest, u32 control);

#define VanillaClearSoftTextMap ((void (*)(void))0x08009A91)
#define VanillaDrawDebugText \
    ((void (*)(const char *, u32, u32, u32, u32, u32))0x08009481)

void ClearSoftTextMap(void)
{
    /* Reference body (baserom @ 0x9A90):
     *   u16 zero = 0;
     *   CpuSet(&zero, gSoftTextMap, CPU_SET_16BIT | CPU_SET_SRC_FIXED | 0x2C0);
     *   gSoftTextDirty |= SOFT_TEXT_DIRTY_BIT;
     */
    VanillaClearSoftTextMap();
}

void DrawDebugText(const char *str, u32 x, u32 y, u32 palBank, u32 a, u32 b)
{
    /* Reference: writes palette-banked tile entries into gSoftTextMap at
     * (x, y), special-casing space and a few high-bit glyphs, then marks
     * gSoftTextDirty bit 8. Full glyph decode lives at 0x08009480. */
    VanillaDrawDebugText(str, x, y, palBank, a, b);
}

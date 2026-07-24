#include "runtime.h"

/*
 * Relocated OnImpact jump table + impact description strings.
 * apply_lynjump.py fills these from baserom / impact_suction.json.
 */
APPEND_RODATA u32 gImpactJumpTable[29] = {0};
APPEND_RODATA u8 gImpactDescTable[29 * 0x5A] = {0};

/* Extended shooter ANM #230 (gun icons) with Suction frames 196/197.
 * Filled by apply_lynjump.py; size written to gGunIconAnmExtSize.
 * Must be word-aligned: the ANM loader uses 32-bit loads (GBA rotates
 * unaligned LDR data → barcode garbage icons). */
#define GUN_ICON_ANM_EXT_MAX 0xE820 /* 59424; rebuilt ANM is 59416 */
APPEND_RODATA u8 gGunIconAnmExt[GUN_ICON_ANM_EXT_MAX] __attribute__((aligned(4))) = {0};
APPEND_DATA u32 gGunIconAnmExtSize = 0;

#include "global.h"
#include "save.h"
#include "ram_map.h"

/*
 * WriteSave / SetSaveSlot / FlushSaveMeta — wrappers for baserom.
 *
 * Linked into append (not peeled). WriteSave @ 0x0800EC10 gates on
 * gSaveBusy, packs the slot checksum into EWRAM @ 0x02000A88, then calls
 * the EEPROM writer. SetSaveSlot + FlushSaveMeta match the END SAVE UI
 * prelude before WriteSave.
 */

#define VanillaWriteSave ((void (*)(void))0x0800EC11)
#define VanillaSetSaveSlot ((void (*)(u32))0x0800EBAD)
#define VanillaFlushSaveMeta ((void (*)(void))0x0800EBC5)

void WriteSave(void)
{
    VanillaWriteSave();
}

void SetSaveSlot(u32 slot)
{
    VanillaSetSaveSlot(slot);
}

void FlushSaveMeta(void)
{
    VanillaFlushSaveMeta();
}

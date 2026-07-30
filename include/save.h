#ifndef GUARD_SAVE_H
#define GUARD_SAVE_H

#include "gba/types.h"

/*
 * Save helpers used by the anywhere-save debug option.
 *
 * WriteSave   @ 0x0800EC10 — EEPROM commit of the active slot
 * SetSaveSlot @ 0x0800EBAC — select slot (writes gSaveSlot, primes writer)
 * FlushSaveMeta @ 0x0800EBC4 — END-SAVE prep before WriteSave
 */

void WriteSave(void);
void SetSaveSlot(u32 slot);
void FlushSaveMeta(void);

#endif /* GUARD_SAVE_H */

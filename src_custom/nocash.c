#include "global.h"
#include "nocash.h"

/*
 * No$GBA debug string ports:
 *   0x4FFFA10  pret: auto newline (flaky on some builds)
 *   0x4FFFA14  pret: raw
 *   0x4FFFA18  Skill System DebugKit
 *
 * Must pass a pointer that stays valid after return — No$ may read the
 * string asynchronously. Stack buffers go stale; use static storage.
 * Append '\n' ourselves and write once (multi-port = duplicate lines).
 */
#define NOCASHGBAPRINTADDR ((volatile u32 *)0x04FFFA18)

APPEND_TEXT void NoCashGBAPrint(const char *pBuf)
{
    static char sBuf[0x100];
    u32 i = 0;

    if (pBuf != NULL)
    {
        while (pBuf[i] != '\0' && i < (sizeof(sBuf) - 2))
        {
            sBuf[i] = pBuf[i];
            i++;
        }
    }

    if (i == 0 || sBuf[i - 1] != '\n')
        sBuf[i++] = '\n';
    sBuf[i] = '\0';

    *NOCASHGBAPRINTADDR = (u32)sBuf;
}

APPEND_TEXT void NoCashGBAPrintf(const char *pBuf, ...)
{
    NoCashGBAPrint(pBuf);
}

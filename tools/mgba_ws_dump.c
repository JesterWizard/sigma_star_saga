/* Boot the ROM, run N frames, dump GAX workspace + params, list ROM pointers. */
#include <mgba/core/core.h>
#include <mgba-util/vfs.h>
#include <mgba/internal/gba/gba.h>

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define WS_PTR     0x0300775C
#define GAX_PARAMS 0x030008C0

int main(int argc, char** argv)
{
    const char* rom = argc > 1 ? argv[1] : "sigma_star_saga.gba";
    int maxFrames = argc > 2 ? atoi(argv[2]) : 400;

    struct mCore* core = mCoreFind(rom);
    if (!core)
        return 1;
    core->init(core);
    mCoreInitConfig(core, NULL);
    if (!core->loadROM(core, VFileOpen(rom, O_RDONLY)))
        return 1;
    core->reset(core);

    for (int f = 0; f < maxFrames; f++) {
        core->runFrame(core);
        core->setKeys(core, (f % 60) < 2 ? 0x9 : 0);
    }

    uint32_t ws = core->busRead32(core, WS_PTR);
    printf("workspace @ %08X\n", ws);
    printf("--- params @ %08X ---\n", GAX_PARAMS);
    for (int off = 0; off < 0x40; off += 4)
        printf("  +%02X: %08X\n", off, core->busRead32(core, GAX_PARAMS + off));
    printf("--- workspace ROM pointers (first 0x1000) ---\n");
    for (int off = 0; off < 0x1000; off += 4) {
        uint32_t v = core->busRead32(core, ws + off);
        if (v >= 0x08000000 && v < 0x0A000000)
            printf("  ws+%03X: %08X\n", off, v);
    }
    printf("--- workspace scalar fields (first 0x100) ---\n");
    for (int off = 0; off < 0x100; off += 4)
        printf("  ws+%02X: %08X\n", off, core->busRead32(core, ws + off));
    return 0;
}

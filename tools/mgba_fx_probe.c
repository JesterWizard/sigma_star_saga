/* Boot, then poke FX channel 0 the way GaxFxRom(id) does, and dump the
 * channel struct while the engine initializes it — reveals id -> sample
 * resolution. Also dumps any natural SFX activity. */
#include <mgba/core/core.h>
#include <mgba-util/vfs.h>
#include <mgba/internal/gba/gba.h>

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define WS_PTR 0x0300775C

static FILE* mix;

static void dumpCh(struct mCore* core, uint32_t ch, int f, const char* tag)
{
    printf("f%03d %s ch@%08X:", f, tag, ch);
    for (int off = 0; off < 0x50; off += 4)
        printf(" %08X", core->busRead32(core, ch + off));
    printf("\n");
}

int main(int argc, char** argv)
{
    const char* rom = argc > 1 ? argv[1] : "sigma_star_saga.gba";
    int pokeId = argc > 2 ? atoi(argv[2]) : 1;

    struct mCore* core = mCoreFind(rom);
    if (!core)
        return 1;
    core->init(core);
    mCoreInitConfig(core, NULL);
    if (!core->loadROM(core, VFileOpen(rom, O_RDONLY)))
        return 1;
    core->reset(core);

    for (int f = 0; f < 640; f++) {
        core->runFrame(core);
        core->setKeys(core, (f % 90) < 2 ? 0x9 : 0);
        uint32_t ws = core->busRead32(core, WS_PTR);
        if (!ws)
            continue;
        uint32_t chArr = core->busRead32(core, ws + 0x0C);
        int nCh = core->busRead32(core, ws + 0x10);

        if (f == 400 && pokeId >= 0 && chArr && nCh > 0) {
            /* GaxFxRom(id, pan): pick channel 0, mirror its exact writes */
            core->busWrite8(core, chArr + 0x4B, 8);
            core->busWrite8(core, chArr + 0x49, 0);
            core->busWrite8(core, chArr + 0x4A, 0);
            core->busWrite8(core, chArr + 0x4C, pokeId);
            core->busWrite32(core, chArr + 0x40, 0);
            core->busWrite8(core, chArr + 0x48, 0xFF);
            core->busWrite8(core, ws + 0x88 + 0, pokeId);
            printf("== poked FX id=%d on ch0 @ %08X ==\n", pokeId, chArr);
        }
        if (f >= 400 && f <= 404 && chArr)
            dumpCh(core, chArr, f, "poke");
        if (f == 401) {
            char name[64];
            snprintf(name, sizeof(name), "/tmp/fx_id%d.s16", pokeId);
            mix = fopen(name, "wb");
        }
        if (mix) {
            uint8_t mb[0x210];
            for (int i = 0; i < 0x210; i++)
                mb[i] = core->busRead8(core, 0x03005A48 + i);
            fwrite(mb, 1, sizeof(mb), mix);
            if (f >= 500) {
                fclose(mix);
                mix = NULL;
            }
        }
        /* natural activity: any channel with +0x4C nonzero */
        if (chArr && f < 400) {
            for (int c = 0; c < nCh && c < 4; c++) {
                uint32_t ch = chArr + c * 0x50;
                int id = core->busRead8(core, ch + 0x4C);
                int active = core->busRead16(core, ch + 0x10);
                if (id || active)
                    dumpCh(core, ch, f, "nat");
            }
        }
    }
    return 0;
}

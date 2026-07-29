/* Headless mGBA harness: plant EWRAM canaries in the provisional free band
 * (0x02005E00–0x02030000), drive title→talk, report write high-water. */
#include <mgba/core/core.h>
#include <mgba-util/vfs.h>
#include <mgba/internal/gba/gba.h>
#include <mgba/internal/arm/arm.h>

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CANARY_LO   0x02005E00u
#define CANARY_HI   0x0202B000u /* FreeEwramSpaceTop — custom pool starts here */
#define CANARY_STRIDE 0x40u
#define CANARY_MARK 0xCAu

static struct mCore *core;

static void plantCanaries(void)
{
    uint32_t a;
    for (a = CANARY_LO; a < CANARY_HI; a += CANARY_STRIDE)
        core->busWrite8(core, a, (uint8_t)(CANARY_MARK ^ ((a >> 8) & 0xFF)));
}

static void scanCanaries(uint32_t *firstDirty, uint32_t *lastDirty, int *nDirty)
{
    uint32_t a;
    *firstDirty = 0;
    *lastDirty = 0;
    *nDirty = 0;
    for (a = CANARY_LO; a < CANARY_HI; a += CANARY_STRIDE) {
        uint8_t expect = (uint8_t)(CANARY_MARK ^ ((a >> 8) & 0xFF));
        uint8_t got = core->busRead8(core, a);
        if (got != expect) {
            if (*nDirty == 0)
                *firstDirty = a;
            *lastDirty = a;
            (*nDirty)++;
        }
    }
}

int main(int argc, char **argv)
{
    const char *rom = argc > 1 ? argv[1] : "sigma_star_saga.gba";
    int maxFrames = argc > 2 ? atoi(argv[2]) : 1200;
    uint32_t firstDirty = 0, lastDirty = 0;
    int nDirty = 0;
    int planted = 0;
    int curFrame;

    core = mCoreFind(rom);
    if (!core)
        return 1;
    core->init(core);
    mCoreInitConfig(core, NULL);
    if (!core->loadROM(core, VFileOpen(rom, O_RDONLY)))
        return 1;
    core->reset(core);

    {
        unsigned vw = 240, vh = 160;
        uint32_t *vbuf;
        core->desiredVideoDimensions(core, &vw, &vh);
        vbuf = malloc(vw * vh * 4);
        core->setVideoBuffer(core, vbuf, vw);
    }

    printf("probe band %08X–%08X stride=0x%X\n", CANARY_LO, CANARY_HI,
           CANARY_STRIDE);

    for (curFrame = 0; curFrame < maxFrames; curFrame++) {
        core->runFrame(core);
        /* Mash START+A like the FX voice harness. */
        core->setKeys(core, (curFrame % 90) < 2 ? 0x9 : 0);

        /* Plant after boot DMA-clear of EWRAM (give a few dozen frames). */
        if (!planted && curFrame == 90) {
            plantCanaries();
            planted = 1;
            printf("f%04d planted canaries\n", curFrame);
        }
    }

    if (!planted) {
        fprintf(stderr, "never planted canaries\n");
        return 2;
    }
    scanCanaries(&firstDirty, &lastDirty, &nDirty);
    printf("dirty slots=%d first=%08X last=%08X\n", nDirty, firstDirty,
           lastDirty);
    if (nDirty == 0) {
        printf("RECOMMEND FreeEwramSpaceTop <= 0x0202C000 (no writes in probe band)\n");
        printf("SAFE_HIGH_WATER=0x%08X\n", CANARY_LO);
    } else {
        /* Floor must sit above last dirty page; leave 0x1000 margin. */
        uint32_t floor = (lastDirty + CANARY_STRIDE + 0xFFF) & ~0xFFFu;
        printf("SAFE_HIGH_WATER=0x%08X\n", lastDirty + CANARY_STRIDE);
        printf("RECOMMEND FreeEwramSpaceTop >= 0x%08X (with margin)\n", floor);
        if (floor > 0x0202C000u)
            printf("WARN: floor 0x%08X leaves less than 80 KiB below 0x02040000\n",
                   floor);
        else
            printf("OK: can set FreeEwramSpaceTop to 0x0202C000 for 80 KiB buffer\n");
    }
    return 0;
}

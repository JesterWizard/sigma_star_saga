/* Headless mGBA harness: boot the ROM (boot-triggered Tierney voice via
 * GaxBootInit__Replacement), watch the speech object per frame, and catch
 * CPU hangs with a wall-clock alarm that dumps ARM state. */
#include <mgba/core/core.h>
#include <mgba-util/vfs.h>
#include <mgba/internal/gba/gba.h>
#include <mgba/internal/arm/arm.h>

#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define SPEECH_OBJ  0x03004348
#define WS_PTR      0x0300775C
#define PROBE_COUNT 0x03004AEC
#define INSTALLED   0x03004AEC
#define GAX_PARAMS  0x030008C0
#define GAX_PARAMS_FLAGS_OFF 0x0E

static struct mCore* core;
static struct ARMCore* cpu;
static int curFrame;

static void dumpScreen(int frame, uint32_t* buf, unsigned w, unsigned h)
{
    char name[64];
    snprintf(name, sizeof(name), "/tmp/gba_f%04d.ppm", frame);
    FILE* f = fopen(name, "wb");
    fprintf(f, "P6\n%u %u\n255\n", w, h);
    for (unsigned i = 0; i < w * h; i++) {
        uint32_t p = buf[i];
        fputc((p >> 19) & 0xF8, f); /* 0xAARRGGBB -> rgb */
        fputc((p >> 11) & 0xF8, f);
        fputc((p >> 3) & 0xF8, f);
    }
    fclose(f);
}

static void onAlarm(int sig)
{
    (void)sig;
    uint32_t pc = cpu->gprs[ARM_PC];
    uint32_t lr = cpu->gprs[ARM_LR];
    fprintf(stderr,
            "\n!! HANG at frame %d: pc=%08X lr=%08X sp=%08X cpsr=%08X\n",
            curFrame, pc, lr, cpu->gprs[ARM_SP], cpu->cpsr.packed);
    for (int i = -4; i < 8; i += 4)
        fprintf(stderr, "   [%08X] = %08X\n", pc + i,
                core->busRead32(core, pc + i));
    uint32_t idx = core->busRead32(core, SPEECH_OBJ + 0x798);
    fprintf(stderr,
            "   speech idx=%d fc=%u bi=%u probe=%u\n",
            (int32_t)idx,
            core->busRead32(core, SPEECH_OBJ + 0x79C),
            core->busRead32(core, SPEECH_OBJ + 0x7A0),
            core->busRead8(core, PROBE_COUNT));
    exit(2);
}

int main(int argc, char** argv)
{
    const char* rom = argc > 1 ? argv[1] : "sigma_star_saga.gba";
    int maxFrames = argc > 2 ? atoi(argv[2]) : 1800;

    core = mCoreFind(rom);
    if (!core) {
        fprintf(stderr, "no core for %s\n", rom);
        return 1;
    }
    core->init(core);
    mCoreInitConfig(core, NULL);
    if (!core->loadROM(core, VFileOpen(rom, O_RDONLY))) {
        fprintf(stderr, "loadROM failed\n");
        return 1;
    }
    core->reset(core);
    cpu = ((struct GBA*)core->board)->cpu;

    unsigned vw = 240, vh = 160;
    core->desiredVideoDimensions(core, &vw, &vh);
    uint32_t* vbuf = malloc(vw * vh * 4);
    core->setVideoBuffer(core, vbuf, vw);

    struct sigaction sa = {0};
    sa.sa_handler = onAlarm;
    sigaction(SIGALRM, &sa, NULL);

    int32_t lastIdx = -2;
    uint32_t lastFc = 0, lastBi = 0, lastFl = 0xFFFF, lastInst = 0xFFFF;
    bool sawActive = false;
    int doneFrame = -1;
    int stuckFcFrames = 0;
    FILE* mixdump = NULL;

    for (curFrame = 0; curFrame < maxFrames; curFrame++) {
        struct itimerval it = {0};
        it.it_value.tv_sec = 5;
        setitimer(ITIMER_REAL, &it, NULL);
        core->runFrame(core);
        it.it_value.tv_sec = 0;
        setitimer(ITIMER_REAL, &it, NULL);

        /* mash START/A to get past logos/menus to the title screen */
        core->setKeys(core, (curFrame % 60) < 2 ? 0x9 : 0);
        if (curFrame == 120 || curFrame == 400 || curFrame == 780)
            dumpScreen(curFrame, vbuf, vw, vh);

        /* trigger voice: replicate gax_fx(0x101) speech path — flag + 3 fields */
        if (curFrame == 400 && (argc < 4 || atoi(argv[3]) != 0)) {
            uint16_t fl = core->busRead16(core, GAX_PARAMS + GAX_PARAMS_FLAGS_OFF);
            core->busWrite16(core, GAX_PARAMS + GAX_PARAMS_FLAGS_OFF, fl | 0x40);
            core->busWrite32(core, SPEECH_OBJ + 0x798, 1);
            core->busWrite32(core, SPEECH_OBJ + 0x79C, 0);
            core->busWrite32(core, SPEECH_OBJ + 0x7A0, 0);
            printf("== voice triggered at frame 400 ==\n");
        }
        if (curFrame == 400)
            mixdump = fopen(argc > 4 ? argv[4] : "/tmp/mixdump.s16", "wb");
        if (mixdump) {
            uint8_t mb[0x210];
            for (int i = 0; i < 0x210; i++)
                mb[i] = core->busRead8(core, 0x03005A48 + i);
            fwrite(mb, 1, sizeof(mb), mixdump);
            if (curFrame >= 680) {
                fclose(mixdump);
                mixdump = NULL;
            }
        }

        int32_t idx = (int32_t)core->busRead32(core, SPEECH_OBJ + 0x798);
        uint32_t fc = core->busRead32(core, SPEECH_OBJ + 0x79C);
        uint32_t bi = core->busRead32(core, SPEECH_OBJ + 0x7A0);
        uint32_t ws = core->busRead32(core, WS_PTR);
        uint32_t probe = core->busRead8(core, PROBE_COUNT);
        uint32_t installed = core->busRead8(core, INSTALLED);
        uint32_t flags = core->busRead16(core, GAX_PARAMS + GAX_PARAMS_FLAGS_OFF);

        if (idx != lastIdx || fc != lastFc || flags != lastFl || installed != lastInst) {
            printf("f%04d idx=%d fc=%u bi=%u probe=%u inst=%u fl=%04X ws=%08X ws14=%08X ws2C=%08X ws5E=%d\n",
                   curFrame, (int)idx, fc, bi, probe, installed, flags, ws,
                   ws ? core->busRead32(core, ws + 0x14) : 0,
                   ws ? core->busRead32(core, ws + 0x2C) : 0,
                   ws ? core->busRead8(core, ws + 0x5E) : 0);
            fflush(stdout);
        }
        if (idx >= 0)
            sawActive = true;
        if (sawActive && idx >= 0 && fc == lastFc && bi == lastBi)
            stuckFcFrames++;
        else
            stuckFcFrames = 0;
        if (sawActive && stuckFcFrames == 240) {
            uint32_t pc = cpu->gprs[ARM_PC];
            printf("!! speech cursors stuck 240 frames (idx=%d fc=%u bi=%u) pc=%08X\n",
                   (int)idx, fc, bi, pc);
            printf("   mixer may have stopped consuming speech\n");
        }
        if (sawActive && idx == -1 && doneFrame < 0) {
            doneFrame = curFrame;
            printf("== speech completed at frame %d (no hang) ==\n", curFrame);
        }
        lastIdx = idx;
        lastFc = fc;
        lastBi = bi;
        lastFl = flags;
        lastInst = installed;
    }
    printf("done: sawActive=%d doneFrame=%d\n", sawActive, doneFrame);
    return 0;
}

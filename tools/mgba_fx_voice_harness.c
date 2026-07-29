/* Headless mGBA harness for the FX voice path: boot the ROM, mash START/A,
 * watch FX channels for the voice FX id (>= 124) plus vanilla SFX activity,
 * dump the mix buffer while voice plays, and catch CPU hangs. */
#include <mgba/core/core.h>
#include <mgba/core/blip_buf.h>
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

#define WS_PTR        0x0300775C
#define GAX_PARAMS    0x030008C0
#define GAX_PARAMS_PACKAGE_OFF 0x30
#define MIX_BUF       0x03005A48
#define VOICE_FX_BASE 124
/* gGaxSpeechObject(0x03004348) + GAX_SPEECH_OBJECT_BYTES(0x7A4) + 2 */
#define VOICE_FX_CH   0x03004AEE

static struct mCore* core;
static struct ARMCore* cpu;
static int curFrame;

static void dumpScreen(int frame, uint32_t* buf, unsigned w, unsigned h)
{
    char name[64];
    snprintf(name, sizeof(name), "/tmp/gba_fx_f%04d.ppm", frame);
    FILE* f = fopen(name, "wb");
    fprintf(f, "P6\n%u %u\n255\n", w, h);
    for (unsigned i = 0; i < w * h; i++) {
        uint32_t p = buf[i];
        fputc((p >> 19) & 0xF8, f);
        fputc((p >> 11) & 0xF8, f);
        fputc((p >> 3) & 0xF8, f);
    }
    fclose(f);
}

static void onAlarm(int sig)
{
    (void)sig;
    uint32_t pc = cpu->gprs[ARM_PC];
    fprintf(stderr,
            "\n!! HANG at frame %d: pc=%08X lr=%08X sp=%08X cpsr=%08X\n",
            curFrame, pc, cpu->gprs[ARM_LR], cpu->gprs[ARM_SP],
            cpu->cpsr.packed);
    for (int i = -4; i < 8; i += 4)
        fprintf(stderr, "   [%08X] = %08X\n", pc + i,
                core->busRead32(core, pc + i));
    exit(2);
}

static int scanChannels(uint32_t ws, bool verbose)
{
    uint32_t chArr = core->busRead32(core, ws + 0x0C);
    int nCh = core->busRead16(core, ws + 0x10);
    int voiceCh = -1;
    if (!chArr || nCh <= 0 || nCh > 16)
        return -1;
    for (int c = 0; c < nCh; c++) {
        uint32_t ch = chArr + c * 0x50;
        int id = core->busRead8(core, ch + 0x4C);
        int active = core->busRead16(core, ch + 0x10);
        if (id == 0 && active == 0)
            continue;
        if (verbose)
            printf("f%04d ch%d @%08X id=%d +10=%04X +40=%08X +48=%02X\n",
                   curFrame, c, ch, id, active,
                   core->busRead32(core, ch + 0x40),
                   core->busRead8(core, ch + 0x48));
        if (id >= VOICE_FX_BASE)
            voiceCh = c;
    }
    return voiceCh;
}

static FILE* gAudioOut;

static void onAudioBuffer(struct mAVStream* stream, struct blip_t* left,
                          struct blip_t* right)
{
    int16_t buf[4096];
    int avail = blip_samples_avail(left);
    while (avail > 0) {
        int n = blip_read_samples(left, buf, avail > 4096 ? 4096 : avail, 0);
        if (n <= 0)
            break;
        if (gAudioOut)
            fwrite(buf, 2, n, gAudioOut);
        avail -= n;
    }
    avail = blip_samples_avail(right);
    while (avail > 0) {
        int n = blip_read_samples(right, buf, avail > 4096 ? 4096 : avail, 0);
        if (n <= 0)
            break;
        avail -= n;
    }
}

int main(int argc, char** argv)
{
    const char* rom = argc > 1 ? argv[1] : "sigma_star_saga.gba";
    int maxFrames = argc > 2 ? atoi(argv[2]) : 1200;

    core = mCoreFind(rom);
    if (!core)
        return 1;
    core->init(core);
    mCoreInitConfig(core, NULL);
    if (!core->loadROM(core, VFileOpen(rom, O_RDONLY)))
        return 1;
    gAudioOut = fopen("/tmp/fx_voice_mix.s16", "wb");
    static struct mAVStream stream;
    stream.postAudioBuffer = onAudioBuffer;
    core->setAVStream(core, &stream);
    core->reset(core);
    cpu = ((struct GBA*)core->board)->cpu;

    unsigned vw = 240, vh = 160;
    core->desiredVideoDimensions(core, &vw, &vh);
    uint32_t* vbuf = malloc(vw * vh * 4);
    core->setVideoBuffer(core, vbuf, vw);

    struct sigaction sa = {0};
    sa.sa_handler = onAlarm;
    sigaction(SIGALRM, &sa, NULL);

    FILE* mixdump = NULL;
    int voiceStartFrame = -1, voiceEndFrame = -1;
    bool pkgLogged = false;
    int16_t lastFxCh = -2;
    uint32_t ws0 = 0;

    for (curFrame = 0; curFrame < maxFrames; curFrame++) {
        struct itimerval it = {0};
        it.it_value.tv_sec = 5;
        setitimer(ITIMER_REAL, &it, NULL);
        core->runFrame(core);
        it.it_value.tv_sec = 0;
        setitimer(ITIMER_REAL, &it, NULL);

        core->setKeys(core, (curFrame % 90) < 2 ? 0x9 : 0);
        if (curFrame == 300 || curFrame == 600)
            dumpScreen(curFrame, vbuf, vw, vh);

        uint32_t pc = cpu->gprs[ARM_PC];
        if (pc >= 0x04000000 && pc < 0x08000000) {
            printf("!! CRASH frame %d pc=%08X cpsr=%08X\n", curFrame, pc,
                   cpu->cpsr.packed);
            for (int r = 0; r <= 12; r++)
                printf("   r%d=%08X%s", r, cpu->gprs[r], r % 4 == 3 ? "\n" : " ");
            printf("   sp=%08X lr=%08X\n", cpu->gprs[ARM_SP], cpu->gprs[ARM_LR]);
            uint32_t sp = cpu->gprs[ARM_SP];
            for (int s = 0; s < 0x40; s += 4) {
                uint32_t v = core->busRead32(core, sp + s);
                if (v >= 0x08000000 && v < 0x0A000000)
                    printf("   [sp+%02X]=%08X (ret?)\n", s, v);
            }
            printf("   stageCase=%u unk0D80=%u cutStep=%u unk3688=%u\n",
                   core->busRead32(core, 0x03007738),
                   core->busRead8(core, 0x03000D80),
                   core->busRead8(core, 0x03003684),
                   core->busRead32(core, 0x03003688));
            printf("   CSU caller region 0x0802B18C: %02X %02X %02X %02X\n",
                   core->busRead8(core, 0x0802B18C), core->busRead8(core, 0x0802B18D),
                   core->busRead8(core, 0x0802B18E), core->busRead8(core, 0x0802B18F));
            if (ws0) {
                uint32_t chArr = core->busRead32(core, ws0 + 0x0C);
                printf("   ch0 @%08X:", chArr);
                for (int off = 0; off < 0x50; off += 4)
                    printf(" %08X", core->busRead32(core, chArr + off));
                printf("\n");
                printf("   ws+88: %02X %02X\n",
                       core->busRead8(core, ws0 + 0x88),
                       core->busRead8(core, ws0 + 0x89));
            }
            return 4;
        }

        uint32_t ws = core->busRead32(core, WS_PTR);
        if (!ws)
            continue;
        ws0 = ws;
        if (!pkgLogged && curFrame > 60) {
            printf("package=%08X fxMixRate=%d ws=%08X\n",
                   core->busRead32(core, GAX_PARAMS + GAX_PARAMS_PACKAGE_OFF),
                   (int16_t)core->busRead16(core, GAX_PARAMS + 0x0A), ws);
            pkgLogged = true;
        }

        int16_t fxCh = (int16_t)core->busRead16(core, VOICE_FX_CH);
        if (fxCh >= 0) {
            uint32_t st = core->busRead32(core, 0x0300775C);
            if (st)
                core->busWrite8(core, st + 0x59, 1);
        }
        if (fxCh != lastFxCh) {
            printf("f%04d gGaxVoiceFxChannel: %d -> %d\n", curFrame, lastFxCh, fxCh);
            lastFxCh = fxCh;
            if (fxCh >= 0 && ws) {
                uint32_t st = core->busRead32(core, 0x0300775C);
                uint32_t ch = core->busRead32(core, st + 12) + fxCh * 0x50;
                printf("f%04d immediately: fxEn=%02X canary5A=%02X probe=%02X 2C=%08X 40=%08X 48=%02X 4B=%02X 4C=%02X q=%02X\n",
                       curFrame, core->busRead8(core, st + 0x59),
                       core->busRead8(core, st + 0x5A),
                       core->busRead8(core, 0x03004AEC),
                       core->busRead32(core, ch + 0x2C),
                       core->busRead32(core, ch + 0x40),
                       core->busRead8(core, ch + 0x48),
                       core->busRead8(core, ch + 0x4B),
                       core->busRead8(core, ch + 0x4C),
                       core->busRead8(core, st + 0x88 + fxCh));
            }
        }
        if (fxCh >= 0) {
            uint32_t chArr = core->busRead32(core, ws + 0x0C);
            int nCh = core->busRead16(core, ws + 0x10);
            if (chArr && fxCh < nCh) {
                uint32_t ch = chArr + fxCh * 0x50;
                printf("f%04d ch%d raw:", curFrame, fxCh);
                for (int b = 0; b < 0x50; b += 4)
                    printf(" %08X", core->busRead32(core, ch + b));
                printf("\n");
            }
            if (curFrame == 815 || curFrame == 900) {
                uint32_t st = core->busRead32(core, 0x0300775C);
                uint32_t chArr = core->busRead32(core, st + 12);
                int nCh = core->busRead32(core, st + 16);
                printf("f%04d state=%08X chArr=%08X nCh=%d fxq=%02X\n", curFrame,
                       st, chArr, nCh, core->busRead8(core, st + 0x88 + fxCh));
                for (int c = 0; c < nCh && c < 12; c++) {
                    uint32_t ch = chArr + c * 0x50;
                    printf("f%04d ch%d @%08X:", curFrame, c, ch);
                    for (int b = 0; b < 0x50; b += 4)
                        printf(" %08X", core->busRead32(core, ch + b));
                    printf("\n");
                }
            }
        }

        if (curFrame >= 813 && curFrame <= 830) {
            uint32_t st = core->busRead32(core, 0x0300775C);
            uint32_t ch = core->busRead32(core, st + 12);
            printf("f%04d fxEn=%02X q=%02X 2C=%08X 40=%08X 4B=%02X 4C=%02X 0E=%04X 38=%08X\n",
                   curFrame,
                   core->busRead8(core, st + 0x59),
                   core->busRead8(core, st + 0x88),
                   core->busRead32(core, ch + 0x2C),
                   core->busRead32(core, ch + 0x40),
                   core->busRead8(core, ch + 0x4B),
                   core->busRead8(core, ch + 0x4C),
                   core->busRead16(core, ch + 0x0E),
                   core->busRead32(core, ch + 0x38));
        }

        int voiceCh = scanChannels(ws, voiceStartFrame >= 0 || curFrame % 120 == 0);
        if (voiceCh >= 0 && voiceStartFrame < 0) {
            voiceStartFrame = curFrame;
            mixdump = fopen("/tmp/fx_voice_mix.s16", "wb");
            printf("== voice FX active on ch%d at frame %d ==\n", voiceCh, curFrame);
        }
        if (voiceStartFrame >= 0 && voiceCh < 0 && voiceEndFrame < 0
            && curFrame > voiceStartFrame + 10) {
            voiceEndFrame = curFrame;
            printf("== voice FX ended at frame %d (dur %d frames) ==\n",
                   curFrame, curFrame - voiceStartFrame);
        }
    }
    if (mixdump)
        fclose(mixdump);
    printf("done: voiceStart=%d voiceEnd=%d\n", voiceStartFrame, voiceEndFrame);
    return voiceStartFrame >= 0 ? 0 : 3;
}

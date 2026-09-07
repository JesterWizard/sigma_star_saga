/* Generic headless mGBA test harness for tools/regtest/.
 *
 * A thin, script-driven wrapper around libmgba: reads commands on stdin,
 * one per line, executes them against a booted ROM, and prints one result
 * line per command to stdout. All test logic (symbol addresses, assertions,
 * pass/fail) lives in the Python driver — this binary only knows how to run
 * frames, press keys, and read/write GBA memory, so it never needs to change
 * when a test case changes.
 *
 * Commands (one per line on stdin):
 *   BOOT <romPath> [savPath|-]        Load ROM (+ optional .sav), reset.
 *   RUN <frames> <keyMask>            Hold keyMask for <frames> frames.
 *   RUN_UNTIL8 <addr> <value> <maxFrames>   Run with no keys until
 *                                      byte@addr == value or maxFrames hit.
 *   RD8 <addr>  / RD16 <addr>  / RD32 <addr>
 *   WR8 <addr> <value>  / WR16 <addr> <value>  / WR32 <addr> <value>
 *   SHOT <path.ppm>                   Dump the current framebuffer.
 *   QUIT
 *
 * keyMask bits: A=0x001 B=0x002 SELECT=0x004 START=0x008 RIGHT=0x010
 *   LEFT=0x020 UP=0x040 DOWN=0x080 R=0x100 L=0x200
 *
 * Output, one line per command:
 *   OK                                RUN / WR* / BOOT / SHOT succeeded
 *   VAL <hex>                         RD* result
 *   TIMEOUT <framesRun>                RUN_UNTIL8 hit maxFrames first
 *   REACHED <framesRun>                RUN_UNTIL8 saw the target value
 *   ERR <message>                     Any failure
 *
 * Addresses/values are hex, with or without a leading 0x.
 */
#include <mgba/core/core.h>
#include <mgba/core/log.h>
#include <mgba-util/vfs.h>
#include <mgba/internal/gba/gba.h>
#include <mgba/internal/gba/savedata.h>

#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCR_W 240
#define SCR_H 160

static struct mCore* core;
static uint32_t* fb;

/* mGBA's default logger writes BIOS/IO stub noise straight to stdout, which
 * would corrupt this program's line-oriented protocol. Discard it instead
 * of trying to filter it out downstream. */
static void discardLog(struct mLogger* logger, int category, enum mLogLevel level,
                        const char* format, va_list args)
{
    (void)logger;
    (void)category;
    (void)level;
    (void)format;
    (void)args;
}

static struct mLogger sQuietLogger = { .log = discardLog };

static uint32_t parseHex(const char* s)
{
    return (uint32_t)strtoul(s, NULL, 16);
}

static void writePpm(const char* path)
{
    FILE* f = fopen(path, "wb");
    if (!f)
        return;
    fprintf(f, "P6\n%d %d\n255\n", SCR_W, SCR_H);
    for (int i = 0; i < SCR_W * SCR_H; i++) {
        uint32_t p = fb[i];
        unsigned char rgb[3] = { (unsigned char)(p >> 16), (unsigned char)(p >> 8),
                                  (unsigned char)p };
        fwrite(rgb, 1, 3, f);
    }
    fclose(f);
}

static uint32_t keyFromMask(const char* tok)
{
    return parseHex(tok);
}

int main(void)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    mLogSetDefaultLogger(&sQuietLogger);
    char line[1024];

    while (fgets(line, sizeof line, stdin)) {
        char* saveptr = NULL;
        char* cmd = strtok_r(line, " \t\r\n", &saveptr);
        if (!cmd || cmd[0] == '#')
            continue;

        if (!strcmp(cmd, "QUIT")) {
            printf("OK\n");
            break;
        }

        if (!strcmp(cmd, "BOOT")) {
            char* romPath = strtok_r(NULL, " \t\r\n", &saveptr);
            char* savPath = strtok_r(NULL, " \t\r\n", &saveptr);
            if (!romPath) {
                printf("ERR missing romPath\n");
                continue;
            }
            core = mCoreFind(romPath);
            if (!core) {
                printf("ERR mCoreFind failed\n");
                continue;
            }
            core->init(core);
            mCoreInitConfig(core, NULL);
            fb = calloc(SCR_W * SCR_H, sizeof(uint32_t));
            core->setVideoBuffer(core, (void*)fb, SCR_W);
            if (!core->loadROM(core, VFileOpen(romPath, O_RDONLY))) {
                printf("ERR loadROM failed\n");
                continue;
            }
            if (savPath && strcmp(savPath, "-"))
                core->loadSave(core, VFileOpen(savPath, O_RDWR));
            GBASavedataForceType(&((struct GBA*)core->board)->memory.savedata,
                                 SAVEDATA_EEPROM512);
            core->reset(core);
            printf("OK\n");
            continue;
        }

        if (!core) {
            printf("ERR no ROM booted (call BOOT first)\n");
            continue;
        }

        if (!strcmp(cmd, "RUN")) {
            char* framesTok = strtok_r(NULL, " \t\r\n", &saveptr);
            char* keysTok = strtok_r(NULL, " \t\r\n", &saveptr);
            if (!framesTok || !keysTok) {
                printf("ERR RUN needs <frames> <keyMask>\n");
                continue;
            }
            int frames = atoi(framesTok);
            uint32_t keys = keyFromMask(keysTok);
            core->setKeys(core, keys);
            for (int i = 0; i < frames; i++)
                core->runFrame(core);
            printf("OK\n");
            continue;
        }

        if (!strcmp(cmd, "RUN_UNTIL8")) {
            char* addrTok = strtok_r(NULL, " \t\r\n", &saveptr);
            char* valTok = strtok_r(NULL, " \t\r\n", &saveptr);
            char* maxTok = strtok_r(NULL, " \t\r\n", &saveptr);
            if (!addrTok || !valTok || !maxTok) {
                printf("ERR RUN_UNTIL8 needs <addr> <value> <maxFrames>\n");
                continue;
            }
            uint32_t addr = parseHex(addrTok);
            uint8_t want = (uint8_t)parseHex(valTok);
            int maxFrames = atoi(maxTok);
            core->setKeys(core, 0);
            int f;
            for (f = 0; f < maxFrames; f++) {
                if (core->busRead8(core, addr) == want)
                    break;
                core->runFrame(core);
            }
            if (f < maxFrames)
                printf("REACHED %d\n", f);
            else
                printf("TIMEOUT %d\n", f);
            continue;
        }

        if (!strcmp(cmd, "RD8") || !strcmp(cmd, "RD16") || !strcmp(cmd, "RD32")) {
            char* addrTok = strtok_r(NULL, " \t\r\n", &saveptr);
            if (!addrTok) {
                printf("ERR %s needs <addr>\n", cmd);
                continue;
            }
            uint32_t addr = parseHex(addrTok);
            if (!strcmp(cmd, "RD8"))
                printf("VAL %02X\n", core->busRead8(core, addr));
            else if (!strcmp(cmd, "RD16"))
                printf("VAL %04X\n", core->busRead16(core, addr));
            else
                printf("VAL %08X\n", core->busRead32(core, addr));
            continue;
        }

        if (!strcmp(cmd, "WR8") || !strcmp(cmd, "WR16") || !strcmp(cmd, "WR32")) {
            char* addrTok = strtok_r(NULL, " \t\r\n", &saveptr);
            char* valTok = strtok_r(NULL, " \t\r\n", &saveptr);
            if (!addrTok || !valTok) {
                printf("ERR %s needs <addr> <value>\n", cmd);
                continue;
            }
            uint32_t addr = parseHex(addrTok);
            uint32_t val = parseHex(valTok);
            if (!strcmp(cmd, "WR8"))
                core->busWrite8(core, addr, (uint8_t)val);
            else if (!strcmp(cmd, "WR16"))
                core->busWrite16(core, addr, (uint16_t)val);
            else
                core->busWrite32(core, addr, val);
            printf("OK\n");
            continue;
        }

        if (!strcmp(cmd, "SHOT")) {
            char* path = strtok_r(NULL, " \t\r\n", &saveptr);
            if (!path) {
                printf("ERR SHOT needs <path>\n");
                continue;
            }
            writePpm(path);
            printf("OK\n");
            continue;
        }

        printf("ERR unknown command '%s'\n", cmd);
    }

    return 0;
}

/* Timeline probe for the START debug menu boss list.
 *
 * The boss rows start a vanilla battle record (TryStartBattle), so the
 * interesting question is not one snapshot but whether the battle arms and then
 * makes progress: gBattle @ 0x03007460 holds the record pointer, an armed flag
 * and a started flag, and gMode should leave the field once the flight arena
 * takes over. Rows that arm and then sit on the same mode with a black frame
 * never got an arena.
 *
 * Usage: mgba_bossbattle_probe <rom> <sav|-> <outPrefix> <firstRow> <lastRow>
 */
#include <mgba/core/core.h>
#include <mgba/core/serialize.h>
#include <mgba-util/vfs.h>
#include <mgba/internal/gba/gba.h>
#include <mgba/internal/gba/savedata.h>

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCR_W 240
#define SCR_H 160

#define A_gMode 0x03001630
#define A_gSetModeId 0x03001EBC
#define A_gStageCase 0x03007738
#define A_gEnemyRemaining 0x03007080
#define A_gBattle 0x03007460
#define A_gDebugMenuActive 0x0202BA58

#define MODE_JT_BASE 0x0800BC04
#define OVERWORLD_FRAME 0x0800D610

#define KEY_A 0x001
#define KEY_START 0x008
#define KEY_DOWN 0x080
#define KEY_SELECT_L 0x204

static struct mCore* core;
static uint32_t* fb;

static uint8_t rd8(uint32_t a) { return core->busRead8(core, a); }
static uint32_t rd32(uint32_t a) { return core->busRead32(core, a); }

static void run(uint32_t keys, int frames)
{
    for (int i = 0; i < frames; i++) {
        core->setKeys(core, keys);
        core->runFrame(core);
    }
}

static void tap(uint32_t keys)
{
    run(keys, 3);
    run(0, 5);
}

static void writePpm(const char* path)
{
    FILE* f = fopen(path, "wb");
    if (!f) return;
    fprintf(f, "P6\n%d %d\n255\n", SCR_W, SCR_H);
    for (int i = 0; i < SCR_W * SCR_H; i++) {
        uint32_t p = fb[i];
        unsigned char rgb[3] = { (p >> 16) & 0xFF, (p >> 8) & 0xFF, p & 0xFF };
        fwrite(rgb, 1, 3, f);
    }
    fclose(f);
}

static int distinctColors(void)
{
    uint32_t seen[256];
    int n = 0;
    for (int i = 0; i < SCR_W * SCR_H; i += 7) {
        uint32_t p = fb[i] & 0xF8F8F8;
        int j = 0;
        for (; j < n; j++)
            if (seen[j] == p) break;
        if (j == n && n < 256) seen[n++] = p;
    }
    return n;
}

static void sample(const char* tag, int row, int frame)
{
    printf("row %2d f=%4d %-6s gMode=%02X stage=%3lu setMode=%3lu enemies=%u "
           "rec=%08lX armed=%u started=%u wave=%lu colors=%d\n",
           row, frame, tag, rd8(A_gMode), (unsigned long)rd32(A_gStageCase),
           (unsigned long)rd32(A_gSetModeId), rd8(A_gEnemyRemaining),
           (unsigned long)rd32(A_gBattle), rd8(A_gBattle + 4),
           rd8(A_gBattle + 5), (unsigned long)rd32(A_gBattle + 8),
           distinctColors());
    fflush(stdout);
}

int main(int argc, char** argv)
{
    const char* romPath = argc > 1 ? argv[1] : "sigma_star_saga.gba";
    const char* savPath = argc > 2 ? argv[2] : NULL;
    const char* outPrefix = argc > 3 ? argv[3] : "/tmp/bbp";
    int firstRow = argc > 4 ? atoi(argv[4]) : 0;
    int lastRow = argc > 5 ? atoi(argv[5]) : 0;
    /* SELECT+L flight-skip gets to a field mode fast but lands on a degenerate
     * stage (ENEMY_TESTER) that draws nothing; pass 0 to boot normally. */
    int skipFlight = argc > 6 ? atoi(argv[6]) : 1;

    core = mCoreFind(romPath);
    if (!core) { fprintf(stderr, "mCoreFind failed\n"); return 1; }
    core->init(core);
    mCoreInitConfig(core, NULL);
    fb = calloc(SCR_W * SCR_H, sizeof(uint32_t));
    core->setVideoBuffer(core, (void*)fb, SCR_W);
    if (!core->loadROM(core, VFileOpen(romPath, O_RDONLY))) {
        fprintf(stderr, "loadROM failed\n"); return 1;
    }
    if (savPath && strcmp(savPath, "-"))
        core->loadSave(core, VFileOpen(savPath, O_RDWR));
    GBASavedataForceType(&((struct GBA*)core->board)->memory.savedata,
                         SAVEDATA_EEPROM512);
    core->reset(core);

    uint8_t fieldMode = 0;
    int steady = 0;
    for (int frame = 0; frame < 60000; frame++) {
        uint32_t keys = 0;
        int ph = frame % 40;
        if (ph < 4) keys = KEY_A;
        else if (ph >= 10 && ph < 14) keys = KEY_START;
        else if (skipFlight && ph >= 20 && ph < 30) keys = KEY_SELECT_L;
        core->setKeys(core, keys);
        core->runFrame(core);
        uint8_t m = rd8(A_gMode);
        if (skipFlight && (m == 0x84 || m == 0x97)) {
            core->busWrite8(core, 0x03007668, 1);
            core->busWrite8(core, 0x03007080, 0);
            core->busWrite8(core, 0x03007730, 0);
        }
        if (m != 0 && rd32(MODE_JT_BASE + (m - 1) * 4) == OVERWORLD_FRAME) {
            if (++steady >= 60) { fieldMode = m; break; }
        } else {
            steady = 0;
        }
    }
    if (!fieldMode) { fprintf(stderr, "never reached a field mode\n"); return 1; }
    /* The intro drive mashes START, so the overlay is probably up. Close it with
     * START rather than by clearing the flag: only the menu's own close path
     * restores gDisplayCtrlMirror, and without that the "field" is a black
     * overlay and every later frame reads as blank. */
    for (int i = 0; i < 8 && rd8(A_gDebugMenuActive); i++)
        tap(KEY_START);
    run(0, 60);
    if (rd8(A_gDebugMenuActive))
        fprintf(stderr, "warning: overlay still open\n");
    printf("field gMode=%02X stage=%lu colors=%d\n", fieldMode,
           (unsigned long)rd32(A_gStageCase), distinctColors());
    char p0[512];
    snprintf(p0, sizeof p0, "%s_field.ppm", outPrefix);
    writePpm(p0);

    struct VFile* state = VFileMemChunk(NULL, 0);
    mCoreSaveStateNamed(core, state, SAVESTATE_SAVEDATA);

    for (int row = firstRow; row <= lastRow; row++) {
        state->seek(state, 0, SEEK_SET);
        mCoreLoadStateNamed(core, state, SAVESTATE_SAVEDATA);
        core->busWrite8(core, 0x03007668, 0);
        run(0, 4);
        core->busWrite8(core, 0x03007668, 0);

        tap(KEY_START);
        tap(KEY_DOWN);
        tap(KEY_DOWN);
        tap(KEY_A);
        for (int i = 0; i < row; i++)
            tap(KEY_DOWN);
        tap(KEY_A);

        sample("start", row, 0);
        for (int t = 1; t <= 12; t++) {
            run(0, 120);
            sample("run", row, t * 120);
            if (t == 3 || t == 8 || t == 12) {
                char path[512];
                snprintf(path, sizeof path, "%s_row%02d_f%04d.ppm", outPrefix,
                         row, t * 120);
                writePpm(path);
            }
        }
    }
    return 0;
}

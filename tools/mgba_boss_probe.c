/* Headless end-to-end check for the START debug menu boss list.
 *
 * Boots to a walking field mode, snapshots that state, then for each row of the
 * boss submenu replays START / DOWN / A, lets the transition settle and reports
 * gMode, the SetMode label the arena init picked and how much of the frame is
 * actually drawn. A row that never leaves the field mode, or that lands on a
 * blank frame, did not enter its arena.
 *
 * Usage: mgba_boss_probe <rom> <sav|-> <outPrefix> [rowCount]
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
#define A_gPendingMode 0x03000D6C
#define A_gModeInitShadow 0x030038B0
#define A_gEnemyRemaining 0x03007080
#define A_gDebugMenuActive 0x0202BA58
#define A_gDebugMenuScreen 0x0202BA4C
#define A_gDebugMenuCursor 0x0202BA54

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

/* Press-and-release so the menu edge detector sees a fresh press. */
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

/* Distinct-colour count is a cheap "is anything drawn" signal: a black or
 * single-colour frame means the arena never came up. */
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

int main(int argc, char** argv)
{
    const char* romPath = argc > 1 ? argv[1] : "sigma_star_saga.gba";
    const char* savPath = argc > 2 ? argv[2] : NULL;
    const char* outPrefix = argc > 3 ? argv[3] : "/tmp/boss";
    int rows = argc > 4 ? atoi(argv[4]) : 11;
    int sweepLo = argc > 6 ? atoi(argv[5]) : -1;
    int sweepHi = argc > 6 ? atoi(argv[6]) : -1;

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

    /* Drive the intro (A / START / flight-skip) until a walking field mode.
     * The debug menu itself only arms on modes whose main-loop JT slot is
     * OverworldMainFrame, so use the same test here. */
    uint8_t fieldMode = 0;
    int steady = 0;
    for (int frame = 0; frame < 60000; frame++) {
        uint32_t keys = 0;
        int ph = frame % 40;
        if (ph < 4) keys = KEY_A;
        else if (ph >= 10 && ph < 14) keys = KEY_START;
        else if (ph >= 20 && ph < 30) keys = KEY_SELECT_L;
        core->setKeys(core, keys);
        core->runFrame(core);
        uint8_t m = rd8(A_gMode);
        if (m == 0x84 || m == 0x97) {
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
    /* The intro drive mashes START, so make sure the menu is shut first. */
    core->busWrite8(core, A_gDebugMenuActive, 0);
    run(0, 30);
    printf("field gMode=%02X\n", fieldMode);

    struct VFile* state = VFileMemChunk(NULL, 0);
    mCoreSaveStateNamed(core, state, SAVESTATE_SAVEDATA);

    /* Sweep mode: poke gStageCase and re-arm mode-init so EnterStageArena runs
     * again, which is what the menu ends up doing without the key replay. */
    if (sweepLo >= 0) {
        for (int c = sweepLo; c <= sweepHi; c++) {
            state->seek(state, 0, SEEK_SET);
            mCoreLoadStateNamed(core, state, SAVESTATE_SAVEDATA);
            run(0, 4);
            core->busWrite32(core, A_gStageCase, c);
            core->busWrite8(core, A_gModeInitShadow, 0xFF);
            run(0, 240);
            int c0 = distinctColors();
            uint32_t after0 = rd32(A_gStageCase);
            run(0, 600);
            printf("case %3d: gMode=%02X stageCase=%lu->%lu setModeId=%lu "
                   "enemies=%u colors=%d/%d\n",
                   c, rd8(A_gMode), (unsigned long)after0,
                   (unsigned long)rd32(A_gStageCase),
                   (unsigned long)rd32(A_gSetModeId), rd8(A_gEnemyRemaining),
                   c0, distinctColors());
            fflush(stdout);
            char p[512];
            snprintf(p, sizeof p, "%s_case%03d.ppm", outPrefix, c);
            writePpm(p);
        }
        return 0;
    }

    for (int row = 0; row < rows; row++) {
        state->seek(state, 0, SEEK_SET);
        mCoreLoadStateNamed(core, state, SAVESTATE_SAVEDATA);
        /* The intro drive forces the stage-clear flag to skip the opening
         * flight; leave it set and the boss arena clears the instant it loads. */
        core->busWrite8(core, 0x03007668, 0);
        run(0, 4);
        core->busWrite8(core, 0x03007668, 0);

        tap(KEY_START);                 /* open menu */
        tap(KEY_DOWN);                  /* Save -> Warp */
        tap(KEY_DOWN);                  /* Warp -> Boss */
        tap(KEY_A);                     /* enter boss list */
        for (int i = 0; i < row; i++)
            tap(KEY_DOWN);
        uint8_t scr = rd8(A_gDebugMenuScreen);
        uint8_t cur = rd8(A_gDebugMenuCursor);
        tap(KEY_A);                     /* confirm */

        /* Sample a few times: some arenas play an intro before handing over. */
        char path[512];
        for (int t = 0; t < 3; t++) {
            run(0, t == 0 ? 300 : 450);
            printf("row %2d t=%d (screen=%u cursor=%u): gMode=%02X pending=%02X "
                   "stageCase=%lu setModeId=%lu enemies=%u colors=%d\n",
                   row, t, scr, cur, rd8(A_gMode), rd8(A_gPendingMode),
                   (unsigned long)rd32(A_gStageCase),
                   (unsigned long)rd32(A_gSetModeId), rd8(A_gEnemyRemaining),
                   distinctColors());
            fflush(stdout);
            snprintf(path, sizeof path, "%s_row%02d_t%d.ppm", outPrefix, row, t);
            writePpm(path);
        }
    }
    return 0;
}

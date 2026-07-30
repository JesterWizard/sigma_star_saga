/* Report the PC of every write to a set of display registers while the
 * overworld debug menu owns the screen.
 *
 * Usage: mgba_dispcnt_watch <rom> <state> <frames> [reg,reg,...]
 *
 * Loads a field savestate, presses START to open the menu, then installs write
 * watchpoints and prints who writes each register. Defaults to the scroll
 * registers, which is what makes the menu text jitter; pass an explicit list to
 * chase DISPCNT/BGxCNT instead.
 */
#include <mgba/core/core.h>
#include <mgba/core/serialize.h>
#include <mgba-util/vfs.h>
#include <mgba/internal/gba/gba.h>
#include <mgba/internal/arm/arm.h>
#include <mgba/debugger/debugger.h>
#include <mgba/internal/arm/debugger/debugger.h>

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define A_gMode 0x03001630

static struct mCore* core;
static struct ARMCore* cpu;
static struct mDebugger* dbg;
static int watching;

static void noop(struct mDebugger* d) { (void)d; }

static void onEntered(struct mDebugger* debugger, enum mDebuggerEntryReason reason,
                      struct mDebuggerEntryInfo* info)
{
    if (watching && reason == DEBUGGER_ENTER_WATCHPOINT && info) {
        printf("  WRITE %08X <- %04X  from pc=%08X lr=%08X (old %04X)\n",
               info->address, (unsigned)info->type.wp.newValue,
               cpu->gprs[ARM_PC], cpu->gprs[ARM_LR],
               (unsigned)info->type.wp.oldValue);
        fflush(stdout);
    }
    debugger->state = DEBUGGER_RUNNING;
}

static void runFrame(void)
{
    dbg->state = DEBUGGER_RUNNING;
    mDebuggerRunFrame(dbg);
}

int main(int argc, char** argv)
{
    const char* rom = argc > 1 ? argv[1] : "sigma_star_saga.gba";
    const char* statePath = argc > 2 ? argv[2] : NULL;
    int watchFrames = argc > 3 ? atoi(argv[3]) : 4;
    const char* regList = argc > 4 ? argv[4]
                                   : "04000010,04000012,04000014,04000016";

    setvbuf(stdout, NULL, _IONBF, 0);

    core = mCoreFind(rom);
    if (!core) return 1;
    core->init(core);
    mCoreInitConfig(core, NULL);
    uint32_t* fb = calloc(240 * 160, sizeof(uint32_t));
    core->setVideoBuffer(core, (void*)fb, 240);
    if (!core->loadROM(core, VFileOpen(rom, O_RDONLY))) return 1;
    core->reset(core);
    cpu = ((struct GBA*)core->board)->cpu;

    /* A CLI debugger needs a backend we don't have; drive a bare custom one. */
    dbg = calloc(1, sizeof(*dbg));
    dbg->type = DEBUGGER_CUSTOM;
    /* Leave platform NULL: mDebuggerAttach installs the core's own platform,
     * which is already wired to the CPU. */
    dbg->init = noop;
    dbg->deinit = noop;
    dbg->paused = noop;
    dbg->custom = noop;
    dbg->entered = onEntered;
    printf("attachDebugger=%p debuggerPlatform=%p\n",
           (void*)core->attachDebugger, (void*)core->debuggerPlatform);
    mDebuggerAttach(dbg, core);

    if (statePath && strcmp(statePath, "-")
        && mCoreLoadStateNamed(core, VFileOpen(statePath, O_RDONLY),
                               SAVESTATE_SAVEDATA)) {
        for (int i = 0; i < 4; i++) runFrame();
    } else {
        /* No state: drive the intro, forcing the flight to clear, until a
         * walking field mode is reached. */
        int frame = 0;
        for (; frame < 60000; frame++) {
            uint32_t keys = 0;
            int ph = frame % 40;
            if (ph < 4) keys = 0x001;
            else if (ph >= 10 && ph < 14) keys = 0x008;
            else if (ph >= 20 && ph < 30) keys = 0x204;
            core->setKeys(core, keys);
            runFrame();
            uint8_t m = core->busRead8(core, A_gMode);
            if (m == 0x84 || m == 0x97) {
                core->busWrite8(core, 0x03007668, 1);
                core->busWrite8(core, 0x03007080, 0);
                core->busWrite8(core, 0x03007730, 0);
            }
            if ((m >= 4 && m <= 9) || (m >= 15 && m <= 23)) break;
        }
        core->setKeys(core, 0);
        for (int i = 0; i < 60; i++) runFrame();
    }
    printf("ready, gMode=%02X\n", core->busRead8(core, A_gMode));

    /* Open the menu: START edge, then let it settle. */
    core->setKeys(core, 0x008);
    for (int i = 0; i < 3; i++) runFrame();
    core->setKeys(core, 0);
    for (int i = 0; i < 12; i++) runFrame();

    char buf[256];
    snprintf(buf, sizeof buf, "%s", regList);
    for (char* tok = strtok(buf, ","); tok; tok = strtok(NULL, ",")) {
        struct mWatchpoint wp = {0};
        wp.address = (uint32_t)strtoul(tok, NULL, 16);
        wp.segment = -1;
        wp.type = WATCHPOINT_WRITE;
        ssize_t id = dbg->platform->setWatchpoint(dbg->platform, &wp);
        printf("watchpoint @ %08X id=%zd\n", wp.address, (long)id);
    }

    watching = 1;
    for (int f = 0; f < watchFrames; f++) {
        printf("--- frame %d ---\n", f);
        runFrame();
    }
    return 0;
}

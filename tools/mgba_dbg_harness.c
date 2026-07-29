/* Debugger harness: catch the anomalous mid-function entry at 0x0880227E
 * (bl _call_via_r0 inside CutsceneStageUpdate) — a legit call always has
 * r0 = 0x0802C025 (literal pool). Any other r0 means execution arrived
 * with a dirty register (stack corruption / bad return). Dumps CPU state,
 * stack, and the mStackTrace call chain. */
#include <mgba/core/core.h>
#include <mgba-util/vfs.h>
#include <mgba/internal/gba/gba.h>
#include <mgba/internal/arm/arm.h>
#include <mgba/debugger/debugger.h>
#include <mgba/internal/debugger/stack-trace.h>

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BP_ADDR   0x0880227E /* bl _call_via_r0 in CutsceneStageUpdate */
#define EXPECT_R0 0x0802C025 /* Helper_2C024 literal */

static struct mCore* core;
static struct ARMCore* cpu;
static struct mDebugger* dbg;
static int curFrame;

static void dumpState(const char* why)
{
    printf("!! %s at frame %d\n", why, curFrame);
    printf("   pc=%08X cpsr=%08X\n", cpu->gprs[ARM_PC], cpu->cpsr.packed);
    for (int r = 0; r <= 12; r++)
        printf("   r%d=%08X%s", r, cpu->gprs[r], r % 4 == 3 ? "\n" : " ");
    printf("   sp=%08X lr=%08X\n", cpu->gprs[ARM_SP], cpu->gprs[ARM_LR]);
    uint32_t sp = cpu->gprs[ARM_SP];
    for (int s = 0; s < 0x60; s += 4) {
        uint32_t v = core->busRead32(core, sp + s);
        printf("   [sp+%02X]=%08X%s\n", s, v,
               (v >= 0x08000000 && v < 0x0A000000) ? " (code)" : "");
    }
    struct mStackTrace* st = &dbg->stackTrace;
    size_t depth = mStackTraceGetDepth(st);
    printf("   stack trace (%zu frames):\n", depth);
    for (size_t i = 0; i < depth; i++) {
        struct mStackFrame* f = mStackTraceGetFrame(st, (uint32_t)i);
        printf("     #%zu call=%08X entry=%08X fb=%08X fin=%d\n", i,
               f->callAddress, f->entryAddress, f->frameBaseAddress,
               (int)f->finished);
    }
}

static void onEntered(struct mDebugger* d, enum mDebuggerEntryReason reason,
                      struct mDebuggerEntryInfo* info)
{
    (void)d;
    if (reason == DEBUGGER_ENTER_BREAKPOINT) {
        uint32_t r0 = cpu->gprs[0];
        if (r0 == EXPECT_R0) {
            dbg->state = DEBUGGER_RUNNING; /* legit Helper_2C024 call */
            return;
        }
        dumpState("ANOMALY: mid-function entry, dirty r0");
        exit(4);
    }
    if (reason == DEBUGGER_ENTER_ILLEGAL_OP) {
        dumpState("ILLEGAL OP");
        exit(5);
    }
    dbg->state = DEBUGGER_RUNNING;
}

int main(int argc, char** argv)
{
    const char* rom = argc > 1 ? argv[1] : "sigma_star_saga.gba";
    int maxFrames = argc > 2 ? atoi(argv[2]) : 2000;

    core = mCoreFind(rom);
    if (!core)
        return 1;
    core->init(core);
    mCoreInitConfig(core, NULL);
    if (!core->loadROM(core, VFileOpen(rom, O_RDONLY)))
        return 1;
    core->reset(core);
    cpu = ((struct GBA*)core->board)->cpu;

    dbg = mDebuggerCreate(DEBUGGER_CLI, core);
    if (!dbg) {
        fprintf(stderr, "no debugger\n");
        return 1;
    }
    dbg->entered = onEntered;
    mDebuggerAttach(dbg, core);
    dbg->platform->setStackTraceMode(dbg->platform, STACK_TRACE_ENABLED);

    struct mBreakpoint bp = {0};
    bp.address = BP_ADDR;
    bp.segment = -1;
    bp.type = BREAKPOINT_HARDWARE;
    ssize_t bpId = dbg->platform->setBreakpoint(dbg->platform, &bp);
    printf("breakpoint id=%zd @ %08X\n", bpId, BP_ADDR);

    for (curFrame = 0; curFrame < maxFrames; curFrame++) {
        dbg->state = DEBUGGER_RUNNING;
        mDebuggerRunFrame(dbg);
        core->setKeys(core, (curFrame % 90) < 2 ? 0x9 : 0);
    }
    printf("done: no anomaly in %d frames\n", maxFrames);
    return 0;
}

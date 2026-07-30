/* Headless probe for the overworld START debug menu.
 *
 * Boots the ROM (optionally with a save), replays a scripted key sequence,
 * and dumps display state + framebuffer so the menu overlay can be inspected
 * without a GUI. Key script format, one entry per line:
 *
 *   <frameCount> <keyNames...>     e.g. "30 START" or "120 -"
 *
 * Key names: A B SELECT START RIGHT LEFT UP DOWN R L  ("-" = no keys)
 */
#include <mgba/core/core.h>
#include <mgba/core/serialize.h>
#include <mgba-util/vfs.h>
#include <mgba/internal/gba/gba.h>
#include <mgba/internal/gba/savedata.h>
#include <mgba/internal/gba/renderers/video-software.h>

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCR_W 240
#define SCR_H 160

/* IWRAM / EWRAM symbols (from sigma_star_saga.elf). */
#define A_gMode            0x03001630
#define A_gKeysHeld        0x03001638
#define A_gCameras         0x030009C0
#define CAM_STRIDE         0x84
#define A_gSoftTextDirty   0x03000C9C
#define A_gSoftDispCnt     0x03000BF4
#define A_gSoftOam         0x03001F70
#define A_gStatusMenuOpen  0x030076C4
#define A_gFramePresent    0x03000BDC
#define A_gHudEnable       0x0300715C
#define A_gDebugMenuActive 0x0202BA58
#define A_gDebugMenuStatusTimer 0x0202BA50
#define A_gDebugMenuPrevKeys 0x0202BA48

static struct mCore* core;

static uint32_t rd32(uint32_t a) { return core->busRead32(core, a); }
static uint16_t rd16(uint32_t a) { return core->busRead16(core, a); }
static uint8_t  rd8 (uint32_t a) { return core->busRead8 (core, a); }

static uint32_t keyFromName(const char* n)
{
    if (!strcmp(n, "A")) return 0x001;
    if (!strcmp(n, "B")) return 0x002;
    if (!strcmp(n, "SELECT")) return 0x004;
    if (!strcmp(n, "START")) return 0x008;
    if (!strcmp(n, "RIGHT")) return 0x010;
    if (!strcmp(n, "LEFT")) return 0x020;
    if (!strcmp(n, "UP")) return 0x040;
    if (!strcmp(n, "DOWN")) return 0x080;
    if (!strcmp(n, "R")) return 0x100;
    if (!strcmp(n, "L")) return 0x200;
    return 0;
}

static void dumpRegs(const char* tag)
{
    printf("=== %s ===\n", tag);
    printf("  gMode=%02X  gDebugMenuActive=%02X  gStatusMenuOpen=%02X  gHudEnable=%02X"
           "  statusTimer=%02X prevKeys=%04X\n",
           rd8(A_gMode), rd8(A_gDebugMenuActive), rd8(A_gStatusMenuOpen),
           rd8(A_gHudEnable), rd8(A_gDebugMenuStatusTimer),
           rd16(A_gDebugMenuPrevKeys));
    printf("  DISPCNT=%04X (soft %04X)  gSoftTextDirty=%02X  gFramePresent=%02X\n",
           rd16(0x04000000), rd16(A_gSoftDispCnt), rd8(A_gSoftTextDirty),
           rd8(A_gFramePresent));
    for (int b = 0; b < 4; b++) {
        uint16_t cnt = rd16(0x04000008 + b * 2);
        printf("  BG%dCNT=%04X pri=%d char=%d(0x%06X) scr=%2d(0x%06X)  HOFS=%d VOFS=%d\n",
               b, cnt, cnt & 3, (cnt >> 2) & 3, 0x06000000 + ((cnt >> 2) & 3) * 0x4000,
               (cnt >> 8) & 0x1F, 0x06000000 + ((cnt >> 8) & 0x1F) * 0x800,
               rd16(0x04000010 + b * 4), rd16(0x04000012 + b * 4));
    }
    printf("  BLDCNT=%04X BLDY=%04X MOSAIC=%04X WININ=%04X WINOUT=%04X\n",
           rd16(0x04000050), rd16(0x04000054), rd16(0x0400004C),
           rd16(0x04000048), rd16(0x0400004A));
    for (int c = 0; c < 4; c++) {
        uint32_t cam = A_gCameras + c * CAM_STRIDE;
        printf("  cam%d: flags(+18)=%04X scrBase(+22)=%2d scrollX(+2C)=%08X"
               " (+2E)=%04X scrollY(+30)=%08X (+32)=%04X\n",
               c, rd16(cam + 0x18), rd8(cam + 22), rd32(cam + 0x2C),
               rd16(cam + 0x2E), rd32(cam + 0x30), rd16(cam + 0x32));
    }
    /* How many OBJs are actually visible in hardware OAM? */
    int vis = 0;
    for (int i = 0; i < 128; i++) {
        uint16_t a0 = rd16(0x07000000 + i * 8);
        if ((a0 & 0x0300) != 0x0200) vis++;
    }
    printf("  visible OBJs in OAM: %d\n", vis);
}

/* Count distinct non-zero tile entries per row of a screenbase, so we can see
 * which map rows actually carry content. */
static void dumpMapRows(int screenBase, const char* tag)
{
    uint32_t base = 0x06000000 + screenBase * 0x800;
    printf("  %s (screenbase %d @ %06X) nonzero entries per row:\n    ", tag,
           screenBase, base);
    for (int y = 0; y < 20; y++) {
        int n = 0;
        for (int x = 0; x < 32; x++)
            if (rd16(base + (y * 32 + x) * 2)) n++;
        printf("%2d ", n);
    }
    printf("\n");
}

static void writePpm(const char* path, const uint32_t* fb)
{
    FILE* f = fopen(path, "wb");
    if (!f) return;
    fprintf(f, "P6\n%d %d\n255\n", SCR_W, SCR_H);
    for (int i = 0; i < SCR_W * SCR_H; i++) {
        uint32_t p = fb[i];
        /* mGBA 0.9 default color_t is 0xFFRRGGBB after normalization; probe
         * both orders is unnecessary since we only need structure, not hue. */
        unsigned char rgb[3] = { (p >> 16) & 0xFF, (p >> 8) & 0xFF, p & 0xFF };
        fwrite(rgb, 1, 3, f);
    }
    fclose(f);
}

int main(int argc, char** argv)
{
    const char* romPath = argc > 1 ? argv[1] : "sigma_star_saga.gba";
    const char* savPath = argc > 2 ? argv[2] : NULL;
    const char* scriptPath = argc > 3 ? argv[3] : NULL;
    const char* outPrefix = argc > 4 ? argv[4] : "/tmp/dbgmenu";

    core = mCoreFind(romPath);
    if (!core) { fprintf(stderr, "mCoreFind failed\n"); return 1; }
    core->init(core);
    mCoreInitConfig(core, NULL);

    uint32_t* fb = calloc(SCR_W * SCR_H, sizeof(uint32_t));
    core->setVideoBuffer(core, (void*)fb, SCR_W);

    if (!core->loadROM(core, VFileOpen(romPath, O_RDONLY))) {
        fprintf(stderr, "loadROM failed\n"); return 1;
    }
    if (savPath && strcmp(savPath, "-"))
        core->loadSave(core, VFileOpen(savPath, O_RDWR));
    /* ROM header says EEPROM_V124 with a 512-byte save; pin it so the EEPROM
     * ready poll in WriteSave can actually complete under autodetect. */
    GBASavedataForceType(&((struct GBA*)core->board)->memory.savedata,
                         SAVEDATA_EEPROM512);
    core->reset(core);
    printf("savedata type=%d (0=none 1=SRAM 2=FLASH512 3=FLASH1M 4=EEPROM 5=EEPROM512)\n",
           (int)((struct GBA*)core->board)->memory.savedata.type);

    /* Load key script. */
    struct { int frames; uint32_t keys; char label[64]; } steps[256];
    int nSteps = 0;
    int autoMode = !scriptPath || !strcmp(scriptPath, "auto");
    if (scriptPath && !autoMode) {
        FILE* f = fopen(scriptPath, "r");
        if (!f) { fprintf(stderr, "open script failed\n"); return 1; }
        char line[256];
        while (fgets(line, sizeof line, f) && nSteps < 256) {
            if (line[0] == '#' || line[0] == '\n') continue;
            char* tok = strtok(line, " \t\n");
            if (!tok) continue;
            steps[nSteps].frames = atoi(tok);
            steps[nSteps].keys = 0;
            steps[nSteps].label[0] = 0;
            while ((tok = strtok(NULL, " \t\n"))) {
                steps[nSteps].keys |= keyFromName(tok);
                strncat(steps[nSteps].label, tok,
                        sizeof(steps[nSteps].label) - strlen(steps[nSteps].label) - 2);
                strncat(steps[nSteps].label, ",", 2);
            }
            nSteps++;
        }
        fclose(f);
    }

    int frame = 0;
    uint8_t lastMode = 0xFF;
    char path[512];

    /* Auto mode: drive the intro with A / START / SELECT+L (flight-skip cheat)
     * until gMode lands in an overworld field mode, then save a state. */
    if (autoMode) {
        const char* statePath = argc > 5 ? argv[5] : "/tmp/field.state";
        int stopMode = argc > 6 ? (int)strtol(argv[6], NULL, 16) : -1;
        int limit = 60000;
        for (frame = 0; frame < limit; frame++) {
            uint32_t keys = 0;
            int ph = frame % 40;
            if (ph < 4) keys = 0x001;              /* A */
            else if (ph >= 10 && ph < 14) keys = 0x008;  /* START */
            else if (ph >= 20 && ph < 30) keys = 0x204;  /* SELECT+L */
            core->setKeys(core, keys);
            core->runFrame(core);
            uint8_t m = rd8(A_gMode);
            if (m != lastMode) {
                printf("frame %5d: gMode %02X -> %02X\n", frame, lastMode, m);
                fflush(stdout);
                lastMode = m;
            }
            if (frame % 240 == 0) {
                snprintf(path, sizeof path, "%s_t%05d.ppm", outPrefix, frame);
                writePpm(path, fb);
            }
            /* The intro flight has to be cleared before the game ever hands
             * control to a walking field mode. */
            if (m == 0x84 || m == 0x97) {
                core->busWrite8(core, 0x03007668, 1); /* gStageClearFlag */
                core->busWrite8(core, 0x03007080, 0); /* gEnemyRemaining */
                core->busWrite8(core, 0x03007730, 0); /* gStageClearGate */
            }
            if (stopMode >= 0 && m == stopMode) {
                core->setKeys(core, 0);
                for (int i = 0; i < 90; i++) core->runFrame(core);
                printf("STOP MODE %02X at frame %d\n", m, frame);
                snprintf(path, sizeof path, "%s_stop.ppm", outPrefix);
                writePpm(path, fb);
                dumpRegs("stop mode");
                dumpMapRows(27, "HUD map");
                dumpMapRows((rd16(0x04000008) >> 8) & 0x1F, "BG0 map");

                /* Display/status mirror cluster: find who shadows DISPCNT/BG3CNT. */
                printf("  --- IWRAM display struct 0x03007130..0x030071B0 ---\n");
                for (uint32_t a = 0x03007130; a < 0x030071B0; a += 2) {
                    uint16_t v = rd16(a);
                    if (v)
                        printf("    %08X: %04X%s%s\n", a, v,
                               v == rd16(0x04000000) ? "  <== equals DISPCNT" : "",
                               v == rd16(0x0400000E) ? "  <== equals BG3CNT" : "");
                }
                printf("  --- IWRAM-wide search for DISPCNT (%04X) / BG3CNT (%04X) ---\n",
                       rd16(0x04000000), rd16(0x0400000E));
                for (uint32_t a = 0x03000000; a < 0x03008000; a += 2) {
                    uint16_t v = rd16(a);
                    if (v == rd16(0x04000000)) printf("    DISPCNT-valued @ %08X\n", a);
                    if (v == rd16(0x0400000E)) printf("    BG3CNT-valued  @ %08X\n", a);
                }

                /* Experiment C: drive the status module's own DISPCNT mirror
                 * instead of fighting it, and gate the HUD rebuild. */
                for (int i = 0; i < 8; i++) {
                    core->busWrite16(core, 0x03007194, 0x0300);
                    core->busWrite16(core, A_gSoftDispCnt, 0x0300);
                    core->busWrite8(core, A_gHudEnable, 0);
                    core->runFrame(core);
                }
                printf("  [exp C] after driving 0x03007194: DISPCNT=%04X BG3CNT=%04X\n",
                       rd16(0x04000000), rd16(0x0400000E));
                snprintf(path, sizeof path, "%s_mirror.ppm", outPrefix);
                writePpm(path, fb);

                /* Experiment A: does clearing the HudSync enable byte stop the
                 * HUD from being rebuilt/uploaded? */
                core->busWrite8(core, A_gHudEnable, 0);
                for (int i = 0; i < 10; i++) {
                    core->runFrame(core);
                    core->busWrite8(core, A_gHudEnable, 0);
                }
                snprintf(path, sizeof path, "%s_hudoff.ppm", outPrefix);
                writePpm(path, fb);
                dumpRegs("after gHudEnable=0 x10 frames");
                dumpMapRows(27, "HUD map");

                /* Experiment B: apply debug-menu style display setup and see
                 * which registers the game restores on the next frames. */
                for (int i = 0; i < 5; i++) {
                    core->busWrite16(core, 0x04000008, 0x1E08); /* BG0 scr30 cb2 */
                    core->busWrite16(core, 0x0400000A, 0x1F0B); /* BG1 scr31 cb2 pri3 */
                    core->busWrite16(core, 0x0400000C, 0);
                    core->busWrite16(core, 0x0400000E, 0);
                    core->busWrite16(core, 0x04000000, 0x0300); /* BG0+BG1 only */
                    core->busWrite16(core, A_gSoftDispCnt, 0x0300);
                    printf("  [exp B] wrote menu regs, frame %d\n", i);
                    core->runFrame(core);
                    printf("   after frame: DISPCNT=%04X soft=%04X BG0=%04X BG1=%04X"
                           " BG2=%04X BG3=%04X hudDirty=%02X\n",
                           rd16(0x04000000), rd16(A_gSoftDispCnt), rd16(0x04000008),
                           rd16(0x0400000A), rd16(0x0400000C), rd16(0x0400000E),
                           rd8(A_gSoftTextDirty));
                }
                snprintf(path, sizeof path, "%s_menuregs.ppm", outPrefix);
                writePpm(path, fb);
                return 0;
            }
            if ((m >= 4 && m <= 9) || (m >= 15 && m <= 23)) {
                /* Let the field settle before snapshotting. */
                core->setKeys(core, 0);
                for (int i = 0; i < 60; i++) core->runFrame(core);
                m = rd8(A_gMode);
                if (!((m >= 4 && m <= 9) || (m >= 15 && m <= 23))) continue;
                printf("FIELD MODE %02X reached at frame %d\n", m, frame);
                snprintf(path, sizeof path, "%s_00_field.ppm", outPrefix);
                writePpm(path, fb);
                dumpRegs("A: field before menu");

                uint16_t bg0Before = rd16(0x04000008);
                uint16_t bg1Before = rd16(0x0400000A);
                uint16_t bg3Before = rd16(0x0400000E);
                uint16_t dispBefore = rd16(0x04000000);

                /* Open: START edge. */
                core->setKeys(core, 0x008);
                for (int i = 0; i < 3; i++) core->runFrame(core);
                core->setKeys(core, 0);
                for (int i = 0; i < 12; i++) core->runFrame(core);
                snprintf(path, sizeof path, "%s_01_menu.ppm", outPrefix);
                writePpm(path, fb);
                dumpRegs("B: menu open");
                dumpMapRows(27, "HUD map");

                /* Flicker hunt: per frame, how many text pixels actually reach
                 * the screen, and what the map/palette look like afterwards. */
                for (int i = 0; i < 24; i++) {
                    core->runFrame(core);
                    int lit = 0;
                    for (int y = 0; y < 16; y++)
                        for (int x = 0; x < SCR_W; x++)
                            if (fb[y * SCR_W + x] & 0x00FFFFFF) lit++;
                    uint16_t bg0 = rd16(0x04000008);
                    uint32_t map0 = 0x06000000 + (((bg0 >> 8) & 0x1F) * 0x800);
                    uint32_t map1 = 0x06000000
                                    + (((rd16(0x0400000A) >> 8) & 0x1F) * 0x800);
                    int t0 = 0, t1 = 0;
                    for (int k = 0; k < 32 * 32; k++) {
                        if (rd16(map0 + k * 2)) t0++;
                        if (rd16(map1 + k * 2)) t1++;
                    }
                    /* Glyph tiles live in charbase 2 at tile 31..95, which the
                     * world layers also stream into. */
                    uint32_t cb = 0x06000000 + (((bg0 >> 2) & 3) * 0x4000);
                    uint32_t fontSum = 0;
                    for (uint32_t a = cb + 31 * 32; a < cb + 96 * 32; a += 2)
                        fontSum = fontSum * 31 + rd16(a);
                    uint32_t mapSum = 0;
                    for (int k = 0; k < 32 * 32; k++)
                        mapSum = mapSum * 31 + rd16(map0 + k * 2);
                    struct GBAVideoSoftwareRenderer* sr =
                        (struct GBAVideoSoftwareRenderer*)
                            ((struct GBA*)core->board)->video.renderer;
                    printf("  [flick %02d] litPx=%4d bg0map=%3d bg1map=%3d"
                           " BG0CNT=%04X DISPCNT=%04X fontSum=%08X mapSum=%08X"
                           " bg0(x=%u y=%u en=%d) bg1(x=%u y=%u en=%d)\n",
                           i, lit, t0, t1, bg0, rd16(0x04000000),
                           fontSum, mapSum,
                           sr->bg[0].x, sr->bg[0].y, sr->bg[0].enabled,
                           sr->bg[1].x, sr->bg[1].y, sr->bg[1].enabled);
                }

                /* Close without saving: START edge. */
                core->setKeys(core, 0x008);
                for (int i = 0; i < 3; i++) core->runFrame(core);
                core->setKeys(core, 0);
                for (int i = 0; i < 30; i++) core->runFrame(core);
                snprintf(path, sizeof path, "%s_02_closed.ppm", outPrefix);
                writePpm(path, fb);
                dumpRegs("C: after close (no save)");

                printf("RESTORE CHECK: DISPCNT %04X->%04X %s | BG0 %04X->%04X %s |"
                       " BG1 %04X->%04X %s | BG3 %04X->%04X %s\n",
                       dispBefore, rd16(0x04000000),
                       dispBefore == rd16(0x04000000) ? "OK" : "MISMATCH",
                       bg0Before, rd16(0x04000008),
                       bg0Before == rd16(0x04000008) ? "OK" : "MISMATCH",
                       bg1Before, rd16(0x0400000A),
                       bg1Before == rd16(0x0400000A) ? "OK" : "MISMATCH",
                       bg3Before, rd16(0x0400000E),
                       bg3Before == rd16(0x0400000E) ? "OK" : "MISMATCH");

                /* Walk a little so a frozen field is obvious. */
                core->setKeys(core, 0x010);
                for (int i = 0; i < 40; i++) core->runFrame(core);
                core->setKeys(core, 0);
                for (int i = 0; i < 5; i++) core->runFrame(core);
                snprintf(path, sizeof path, "%s_03_walk.ppm", outPrefix);
                writePpm(path, fb);
                dumpRegs("D: after walking right");

                /* Reopen, save with A, let the status expire, then close. */
                core->setKeys(core, 0x008);
                for (int i = 0; i < 3; i++) core->runFrame(core);
                core->setKeys(core, 0);
                for (int i = 0; i < 12; i++) core->runFrame(core);
                for (int i = 0; i < 5; i++) {
                    printf("  [idle %d] cursorProbe=%02X active=%02X mode=%02X\n",
                           i, rd8(0x0202BA54), rd8(A_gDebugMenuActive), rd8(A_gMode));
                    core->runFrame(core);
                }
                printf("  [save trace] before A: timer=%02X active=%02X\n",
                       rd8(A_gDebugMenuStatusTimer), rd8(A_gDebugMenuActive));
                /* The EEPROM commit blocks for ~1 frame per word, so the menu
                 * hook stays parked for >100 frames before "Saved!" appears. */
                for (int i = 0; i < 240; i++) {
                    core->setKeys(core, i < 1 ? 0x001 : 0);
                    core->runFrame(core);
                    if (i == 30) {
                        snprintf(path, sizeof path, "%s_04a_savedmsg.ppm",
                                 outPrefix);
                        writePpm(path, fb);
                    }
                    if (i % 20) continue;
                    printf("   f%02d keys=%d mode=%02X KEYINPUT=%04X raw=%04X timer=%02X"
                           " active=%02X prev=%04X cursorProbe=%02X\n",
                           i, i < 3, rd8(A_gMode), rd16(0x04000130),
                           (unsigned)(~rd16(0x04000130) & 0x3FF),
                           rd8(A_gDebugMenuStatusTimer),
                           rd8(A_gDebugMenuActive), rd16(A_gDebugMenuPrevKeys),
                           rd8(0x0202BA54));
                    {
                        struct GBASavedata* sd =
                            &((struct GBA*)core->board)->memory.savedata;
                        printf("        eeprom cmd=%d readBits=%d addr=%08X"
                               " wraddr=%08X dirty=%d VCOUNT=%04X DISPCNT=%04X"
                               " IE=%04X IME=%04X\n",
                               (int)sd->command, (int)sd->readBitsRemaining,
                               sd->readAddress, sd->writeAddress, (int)sd->dirty,
                               rd16(0x04000006), rd16(0x04000000),
                               rd16(0x04000200), rd16(0x04000208));
                    }
                    printf("        PC=%08X LR=%08X SP=%08X CPSR=%08X\n",
                           ((struct ARMCore*)core->cpu)->gprs[15],
                           ((struct ARMCore*)core->cpu)->gprs[14],
                           ((struct ARMCore*)core->cpu)->gprs[13],
                           ((struct ARMCore*)core->cpu)->cpsr.packed);
                }
                snprintf(path, sizeof path, "%s_04_saved.ppm", outPrefix);
                writePpm(path, fb);
                dumpRegs("E: after A (save)");
                printf("  gSaveBusy=%02X gSaveSlot=%04X\n",
                       rd8(0x030070BC), rd16(0x030070C0));
                for (int i = 0; i < 120; i++) core->runFrame(core); /* status expires */
                core->setKeys(core, 0x008);
                for (int i = 0; i < 3; i++) core->runFrame(core);
                core->setKeys(core, 0);
                for (int i = 0; i < 30; i++) core->runFrame(core);
                snprintf(path, sizeof path, "%s_05_closed2.ppm", outPrefix);
                writePpm(path, fb);
                dumpRegs("F: after close following save");
                core->setKeys(core, 0x010);
                for (int i = 0; i < 40; i++) core->runFrame(core);
                core->setKeys(core, 0);
                for (int i = 0; i < 5; i++) core->runFrame(core);
                snprintf(path, sizeof path, "%s_06_walk2.ppm", outPrefix);
                writePpm(path, fb);
                dumpRegs("G: walking after save+close");
                return 0;
            }
        }
        printf("no field mode reached in %d frames (last %02X)\n", limit, lastMode);
        return 2;
    }
    for (int s = 0; s < nSteps; s++) {
        core->setKeys(core, steps[s].keys);
        for (int i = 0; i < steps[s].frames; i++) {
            core->runFrame(core);
            frame++;
            uint8_t m = rd8(A_gMode);
            if (m != lastMode) {
                printf("frame %5d: gMode %02X -> %02X (step %d keys=%s)\n",
                       frame, lastMode, m, s,
                       steps[s].label[0] ? steps[s].label : "-");
                lastMode = m;
            }
        }
        snprintf(path, sizeof path, "%s_step%02d.ppm", outPrefix, s);
        writePpm(path, fb);
        snprintf(path, sizeof path, "step %d (keys=%s) after frame %d", s,
                 steps[s].label[0] ? steps[s].label : "-", frame);
        dumpRegs(path);
        dumpMapRows(27, "HUD map");
        {
            uint16_t bg0 = rd16(0x04000008);
            dumpMapRows((bg0 >> 8) & 0x1F, "BG0 map");
        }
        fflush(stdout);
    }
    return 0;
}

#include "global.h"
#include "runtime.h"
#include "ram_map.h"
#include "actor.h"
#include "debug_menu.h"
#include "overworld_frame.h"
#include "save.h"
#include "status.h"
#include "nocash.h"

/*
 * Overworld START debug menu — mode-preserving overlay.
 *
 * Never calls StatusToggle / StatusPanel / SetMode(0x168) / LeaveStatusRestore.
 * Stays in the current overworld gMode; skips world draw while open via the
 * OverworldMainFrame / FrameTail LynJump sites.
 *
 * Display while open:
 *   BG0 — debug font text on CB2 / SB30 (no VRAM overlap; vanilla debug text
 *         uses CB3/SB28 which share the same 16KB and fight the tilemap)
 *   BG1 — solid black fill (blank tile 0 on SB31)
 *   BG2–BG3 / OBJ off; world sim paused
 *
 * Two engine paths run every frame from the main-loop epilogue @ 0x0800BB00,
 * i.e. outside both LynJump sites, so pausing the mode frame does not stop
 * them and the overlay has to cooperate with them rather than fight them:
 *
 *   HudSync(1) @ 0x0800BB98 rebuilds the HUD tilemap (health / bombs / EXP)
 *   into 0x03001670 and sets gSoftTextDirty bit 4; VBlank then uploads it to
 *   screenbase 27. gHudEnabled = 0 makes it early-out.
 *
 *   The status/HUD display module re-applies gDisplayCtrlMirror to REG_DISPCNT
 *   (and restores BG3CNT) every frame. Writing REG_DISPCNT / REG_BG3CNT
 *   directly is therefore pointless — the HUD layer comes straight back. Layer
 *   visibility has to be driven through gDisplayCtrlMirror instead.
 *
 * Deliberately avoids ClearSoftTextMap / DrawDebugText: those set
 * gSoftTextDirty, and VBlank @ 0x39B8 then DMA-reloads soft maps every frame.
 *
 * Restore relies on the engine's own soft tilemaps: the overlay dirties every
 * camera layer on close so VBlank repaints the maps it scribbled on. Only the
 * charbase-2 tiles (clobbered by the font load) need a real snapshot.
 *
 * Scene teleporter: NAV location table @ 0x0824F394 (name[0x20] + modeId).
 * Confirm closes the overlay then QueueMode(modeId) — same deferred ChangeMode
 * path doors / status use. Map prep JT @ 0x0801A1A0 indexes gMode-4.
 */

#ifndef DEBUG_MENU_LOG
#define DEBUG_MENU_LOG 1
#endif

#if DEBUG_MENU_LOG
#define DBG_LOG(msg) NoCashGBAPrint(msg)
#else
#define DBG_LOG(msg) ((void)0)
#endif

#define VRAM_BASE 0x06000000
#define PLTT_BG_ADDR 0x05000000
#define OAM_ADDR 0x07000000

#define ATTR0_HIDE 0x0200
#define DBG_TEXT_PALBANK 15
#define DBG_TEXT_X 1
#define DEBUG_STATUS_FRAMES 90
#define DBG_VISIBLE_ROWS 18

/* gDebugMenuTextState — which string the map currently holds. NONE forces the
 * first repaint after the menu opens. */
#define DBG_TEXT_NONE 0
#define DBG_TEXT_OPTION 1
#define DBG_TEXT_SAVED 2
#define DBG_TEXT_SAVING 3

#define DBG_TEXT_LAYER 3
#define DBG_TEXT_ARCHIVE 3
/* CB2 = 0x06008000..0x0600BFFF; SB30 = 0x0600F000 — no overlap. */
#define DBG_TEXT_SCRBASE 30
#define DBG_TEXT_CHARBASE 2
#define DBG_BLACK_SCRBASE 31
#define DBG_BG0_CNT \
    (BGCNT_PRIORITY(0) | BGCNT_CHARBASE(DBG_TEXT_CHARBASE) \
     | BGCNT_SCREENBASE(DBG_TEXT_SCRBASE))
#define DBG_BG1_CNT \
    (BGCNT_PRIORITY(3) | BGCNT_CHARBASE(DBG_TEXT_CHARBASE) \
     | BGCNT_SCREENBASE(DBG_BLACK_SCRBASE))

#define CAM_STRIDE 0x84
#define CAM_SCROLL_X 0x2C
#define CAM_SCROLL_Y 0x30
#define CAM_ACTIVE 0x18

#define DBG_CLOSED 0
#define DBG_MENU 1

#define DBG_SCR_ROOT 0
#define DBG_SCR_WARP 1

#define DBG_MAGIC_A 0xA5
#define DBG_MAGIC_B 0x5A
#define DBG_MAGIC_C 0xC3

#define DBG_SNAP_FONT_BYTES 0x4000
#define DBG_CAM_COUNT 4
/* Bits 0-3 = camera layers, bit 4 = HUD map: dirty them all to force VBlank
 * to repaint every tilemap the overlay wrote over. */
#define DBG_DIRTY_ALL 0x1F

/* Main-loop mode JT @ 0x0800BC04; overworld field bodies land on D610. */
#define MODE_JT_BASE 0x0800BC04
#define OVERWORLD_FRAME_ADDR 0x0800D610

typedef void (*SetupCamLayerFn)(u32 layer, u32 fileId, u32 scrBase, u32 charBase,
                                u32 a, u32 b);

#define SetupCamLayerGfx ((SetupCamLayerFn)0x08007475)

/* Vanilla NAV location table: u32 modeId + 0x20-byte name (stride 0x24).
 * Name-first parsing pairs each title with the *next* row's mode. */
typedef struct {
    u32 modeId;
    char name[0x20];
} DebugWarpEntry;

#define DEBUG_WARP_TABLE ((const DebugWarpEntry *)0x0824F348)
#define DEBUG_WARP_COUNT 24

enum {
    DBG_OPT_SAVE = 0,
    DBG_OPT_WARP,
    DBG_OPT_COUNT
};

APPEND_RODATA static const char sOptSave[] = "Save game";
APPEND_RODATA static const char sOptWarp[] = "Warp to scene...";
APPEND_RODATA static const char sStatusSaved[] = "Saved!";
APPEND_RODATA static const char sStatusSaving[] = "Saving...";

static bool8 IsOverworldFieldMode(void)
{
    u8 mode = gMode;
    const u32 *jt;

    if (mode == 0)
        return FALSE;
    jt = (const u32 *)MODE_JT_BASE;
    return jt[mode - 1] == OVERWORLD_FRAME_ADDR;
}

static void DebugMenu_ResetState(void)
{
    gDebugMenuActive = DBG_CLOSED;
    gDebugMenuCursor = 0;
    gDebugMenuStatusTimer = 0;
    gDebugMenuScreen = DBG_SCR_ROOT;
    gDebugMenuScroll = 0;
    gDebugMenuPrevKeys = 0;
}

static void DebugMenu_EnsureInit(void)
{
    if (gDebugMenuMagicA == DBG_MAGIC_A
        && gDebugMenuMagicB == DBG_MAGIC_B
        && gDebugMenuMagicC == DBG_MAGIC_C)
    {
        if (gDebugMenuActive > DBG_MENU)
            gDebugMenuActive = DBG_CLOSED;
        return;
    }
    gDebugMenuMagicA = DBG_MAGIC_A;
    gDebugMenuMagicB = DBG_MAGIC_B;
    gDebugMenuMagicC = DBG_MAGIC_C;
    DebugMenu_ResetState();
}

static u16 KeysRaw(void)
{
    /* Direct pad — stays live even if a soft path is stale while paused. */
    return (u16)(~REG_KEYINPUT & KEYS_MASK);
}

static u16 KeysPressed(void)
{
    return (u16)(KeysRaw() & (u16)~gDebugMenuPrevKeys);
}

static void DebugMenu_StorePrevKeys(void)
{
    gDebugMenuPrevKeys = KeysRaw();
}

static vu16 *DebugMenu_ScreenMap(u16 bgcnt)
{
    u32 block = (bgcnt >> 8) & 0x1F;

    return (vu16 *)(VRAM_BASE + block * 0x800);
}

static vu32 *DebugMenu_CharBase(u16 bgcnt)
{
    u32 block = (bgcnt >> 2) & 3;

    return (vu32 *)(VRAM_BASE + block * 0x4000);
}

static void DebugMenu_HideAllSprites(void)
{
    u16 i;
    vu16 *hw = (vu16 *)OAM_ADDR;

    for (i = 0; i < 128; i++)
    {
        u16 *e = &gSoftOam[i * 4];

        e[0] = ATTR0_HIDE;
        e[1] = 0;
        e[2] = 0;
        e[3] = 0;

        hw[i * 4] = ATTR0_HIDE;
        hw[i * 4 + 1] = 0;
        hw[i * 4 + 2] = 0;
        hw[i * 4 + 3] = 0;
    }
    gOamCursor = 0;
    gSoftOamCount = 0;
}

/* Tile pixels only — the debug font load overwrites charbase 2, which the HUD
 * (BG3, charbase 2) and the world layers draw from. Tilemaps are not saved:
 * they are repainted from the engine's soft maps on close. */
static void DebugMenu_SnapVram(void)
{
    u16 i;
    vu32 *font = DebugMenu_CharBase(DBG_BG0_CNT);
    u32 *fdst = (u32 *)gDebugMenuVramSnap;

    for (i = 0; i < (DBG_SNAP_FONT_BYTES / 4); i++)
        fdst[i] = font[i];
}

static void DebugMenu_RestoreVram(void)
{
    u16 i;
    vu32 *font = DebugMenu_CharBase(DBG_BG0_CNT);
    u32 *fsrc = (u32 *)gDebugMenuVramSnap;

    for (i = 0; i < (DBG_SNAP_FONT_BYTES / 4); i++)
        font[i] = fsrc[i];
}

/* The font load re-points camera layer 3 and the overlay zeroes every layer's
 * active flag and scroll; without a full copy the field never comes back. */
static void DebugMenu_SnapCameras(void)
{
    u16 i;

    for (i = 0; i < DBG_CAM_COUNT * CAM_STRIDE; i++)
        gDebugMenuCamSnap[i] = gCameras[i];
}

static void DebugMenu_RestoreCameras(void)
{
    u16 i;

    for (i = 0; i < DBG_CAM_COUNT * CAM_STRIDE; i++)
        gCameras[i] = gDebugMenuCamSnap[i];
}

static void DebugMenu_BlankTile0(void)
{
    vu32 *tile0 = DebugMenu_CharBase(DBG_BG0_CNT);
    u8 t;

    for (t = 0; t < 8; t++)
        tile0[t] = 0;
}

static void DebugMenu_SilenceVblankMaps(void)
{
    u8 i;
    u8 *cam;

    /* Stop soft-map DMA onto VRAM (bits cover cam layers 0–3). */
    gSoftTextDirty = 0;
    gDebugTextActive = 0;
    /* HudSync runs from the main-loop epilogue, outside both LynJump sites. */
    gHudEnabled = 0;

    for (i = 0; i < DBG_CAM_COUNT; i++)
    {
        cam = gCameras + i * CAM_STRIDE;
        *(s32 *)(cam + CAM_SCROLL_X) = 0;
        *(s32 *)(cam + CAM_SCROLL_Y) = 0;
        /* Disable cam-layer update/reload while the menu owns the screen. */
        *(vu16 *)(cam + CAM_ACTIVE) = 0;
    }

    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG3HOFS = 0;
    REG_BG3VOFS = 0;
    gCamScrollMirrorX = 0;
    gCamScrollMirrorY = 0;
}

static void DebugMenu_FillBlackBg1(void)
{
    vu16 *map = DebugMenu_ScreenMap(DBG_BG1_CNT);
    u16 i;

    for (i = 0; i < 32 * 32; i++)
        map[i] = 0;
}

/* Match DrawDebugText @ 0x08009480 glyph index rules. */
static u16 DebugMenu_GlyphEntry(u8 ch)
{
    u16 tile;

    if (ch == ' ')
        return 0;
    if (ch == 0xBF)
        tile = 0x83;
    else if (ch == 0xDF)
        tile = 0x84;
    else if (ch == 0xAE)
        tile = 0x86;
    else if (ch == 0x99)
        tile = 0x87;
    else if (ch <= 0xA1)
        tile = (u16)(ch - 31);
    else
        tile = (u16)(ch - 127);

    return (u16)(tile | (DBG_TEXT_PALBANK << 12));
}

static void DebugMenu_ClearTextMap(void)
{
    vu16 *map = DebugMenu_ScreenMap(DBG_BG0_CNT);
    u16 i;

    for (i = 0; i < 32 * 32; i++)
        map[i] = 0;
}

static void DebugMenu_PutLine(const char *str, u8 x, u8 y)
{
    vu16 *map = DebugMenu_ScreenMap(DBG_BG0_CNT);
    u8 col = x;

    while (*str != '\0' && col < 32)
    {
        map[(u16)y * 32 + col] = DebugMenu_GlyphEntry((u8)*str);
        str++;
        col++;
    }
}

static void DebugMenu_PutSelected(const char *label, u8 y, bool8 selected)
{
    char line[32];
    u8 i = 0;
    u8 j = 0;

    if (selected)
    {
        line[i++] = '>';
        line[i++] = ' ';
    }
    else
    {
        line[i++] = ' ';
        line[i++] = ' ';
    }

    while (label[j] != '\0' && i < 31)
    {
        line[i++] = label[j++];
    }
    line[i] = '\0';
    DebugMenu_PutLine(line, DBG_TEXT_X, y);
}

static u8 DebugMenu_OptionCount(void)
{
    if (gDebugMenuScreen == DBG_SCR_WARP)
        return DEBUG_WARP_COUNT;
    return DBG_OPT_COUNT;
}

static void DebugMenu_ClampScroll(void)
{
    u8 count = DebugMenu_OptionCount();
    u8 maxScroll;

    if (gDebugMenuCursor >= count)
        gDebugMenuCursor = (u8)(count - 1);

    if (count <= DBG_VISIBLE_ROWS)
    {
        gDebugMenuScroll = 0;
        return;
    }

    maxScroll = (u8)(count - DBG_VISIBLE_ROWS);
    if (gDebugMenuCursor < gDebugMenuScroll)
        gDebugMenuScroll = gDebugMenuCursor;
    else if (gDebugMenuCursor >= (u8)(gDebugMenuScroll + DBG_VISIBLE_ROWS))
        gDebugMenuScroll = (u8)(gDebugMenuCursor - DBG_VISIBLE_ROWS + 1);

    if (gDebugMenuScroll > maxScroll)
        gDebugMenuScroll = maxScroll;
}

static void DebugMenu_MoveCursor(s8 delta)
{
    u8 count = DebugMenu_OptionCount();
    s16 next;

    if (count == 0)
        return;

    next = (s16)gDebugMenuCursor + delta;
    if (next < 0)
        next = (s16)(count - 1);
    else if (next >= count)
        next = 0;

    gDebugMenuCursor = (u8)next;
    DebugMenu_ClampScroll();
    gDebugMenuTextState = DBG_TEXT_NONE;
}

static void DebugMenu_LoadFont(void)
{
    SetupCamLayerGfx(DBG_TEXT_LAYER, DBG_TEXT_ARCHIVE, DBG_TEXT_SCRBASE,
                     DBG_TEXT_CHARBASE, 0, 0);
    gDebugMenuTextBg0Cnt = DBG_BG0_CNT;
    DebugMenu_BlankTile0();
    DebugMenu_SilenceVblankMaps();
}

static void DebugMenu_Snapshot(void)
{
    gDebugMenuSavedDisplayCtrl = gDisplayCtrlMirror;
    gDebugMenuSavedHudEnabled = gHudEnabled;
    gDebugMenuSavedDispCnt = gSoftDispCnt;
    /* BGxCNT has no soft mirror — the camera layer setup @ 0x080077D2 writes
     * IO directly, so the live registers are the only source of truth. */
    gDebugMenuSavedBg0Cnt = REG_BG0CNT;
    gDebugMenuSavedBg1Cnt = REG_BG1CNT;
    gDebugMenuSavedBg2Cnt = REG_BG2CNT;
    gDebugMenuSavedBg3Cnt = REG_BG3CNT;
    gDebugMenuSavedBgPal0 = *(vu16 *)PLTT_BG_ADDR;
    gDebugMenuSavedBgPal15 = ((vu16 *)PLTT_BG_ADDR)[DBG_TEXT_PALBANK * 16 + 1];
    gDebugMenuSavedBldCnt = REG_BLDCNT;
    gDebugMenuSavedBldY = REG_BLDY;
    gDebugMenuSavedDispStat = REG_DISPSTAT;
    gDebugMenuSavedIe = REG_IE;
    gDebugMenuSavedWin0H = REG_WIN0H;
    gDebugMenuSavedWin0V = REG_WIN0V;
    gDebugMenuSavedWin1H = REG_WIN1H;
    gDebugMenuSavedWin1V = REG_WIN1V;
    gDebugMenuSavedWinIn = REG_WININ;
    gDebugMenuSavedWinOut = REG_WINOUT;
    gDebugMenuSavedCamScrollX = gCamScrollMirrorX;
    gDebugMenuSavedCamScrollY = gCamScrollMirrorY;
    /* Cameras and charbase-2 tiles must be captured before the font load
     * re-points camera layer 3 and overwrites the shared tiles. */
    gDebugMenuTextBg0Cnt = DBG_BG0_CNT;
    DebugMenu_SnapCameras();
    DebugMenu_SnapVram();
    DebugMenu_LoadFont();
}

static void DebugMenu_RestoreRegs(void)
{
    gDisplayCtrlMirror = gDebugMenuSavedDisplayCtrl;
    gHudEnabled = gDebugMenuSavedHudEnabled;
    gSoftDispCnt = gDebugMenuSavedDispCnt;
    REG_DISPCNT = gDebugMenuSavedDispCnt;
    REG_BG0CNT = gDebugMenuSavedBg0Cnt;
    REG_BG1CNT = gDebugMenuSavedBg1Cnt;
    REG_BG2CNT = gDebugMenuSavedBg2Cnt;
    REG_BG3CNT = gDebugMenuSavedBg3Cnt;
    *(vu16 *)PLTT_BG_ADDR = gDebugMenuSavedBgPal0;
    ((vu16 *)PLTT_BG_ADDR)[DBG_TEXT_PALBANK * 16 + 1] = gDebugMenuSavedBgPal15;
    REG_BLDCNT = gDebugMenuSavedBldCnt;
    REG_BLDY = gDebugMenuSavedBldY;
    REG_DISPSTAT = gDebugMenuSavedDispStat;
    REG_IE = gDebugMenuSavedIe;
    REG_WIN0H = gDebugMenuSavedWin0H;
    REG_WIN0V = gDebugMenuSavedWin0V;
    REG_WIN1H = gDebugMenuSavedWin1H;
    REG_WIN1V = gDebugMenuSavedWin1V;
    REG_WININ = gDebugMenuSavedWinIn;
    REG_WINOUT = gDebugMenuSavedWinOut;
    gCamScrollMirrorX = gDebugMenuSavedCamScrollX;
    gCamScrollMirrorY = gDebugMenuSavedCamScrollY;
    DebugMenu_RestoreCameras();
    DebugMenu_RestoreVram();
    /* The overlay wrote its text map over screenbase 30 and a black fill over
     * screenbase 31; dirtying every layer makes VBlank repaint them from the
     * engine's soft maps. */
    gSoftTextDirty = DBG_DIRTY_ALL;
    gDebugTextActive = 0;
}

/* VRAM/palette/OAM content. Safe to run any time in the frame. */
static void DebugMenu_PrepareSurface(void)
{
    vu16 *pal15;

    DebugMenu_SilenceVblankMaps();
    DebugMenu_BlankTile0();
    DebugMenu_FillBlackBg1();

    ((vu16 *)PLTT_BG_ADDR)[0] = 0;
    pal15 = (vu16 *)PLTT_BG_ADDR + DBG_TEXT_PALBANK * 16;
    pal15[0] = 0;
    pal15[1] = 0x7FFF;

    DebugMenu_HideAllSprites();
}

/* Display registers only. These must be the last writes before scanline 0:
 * the engine's VBlank handler reprograms them every frame, so anything written
 * from the main loop is already stale by the time the frame renders. */
static void DebugMenu_ApplyRegs(void)
{
    u16 bg0 = DBG_BG0_CNT;
    u16 bg1 = DBG_BG1_CNT;
    u16 disp;

    gDebugMenuTextBg0Cnt = bg0;
    REG_BG0CNT = bg0;
    REG_BG1CNT = bg1;
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;

    /* BG2/BG3 and OBJ are switched off by clearing their DISPCNT bits, not by
     * zeroing BGxCNT: the status module rewrites BG3CNT every frame. */
    disp = (u16)(DISPCNT_MODE_0 | DISPCNT_BG0_ON | DISPCNT_BG1_ON);
    gDisplayCtrlMirror = disp;
    gSoftDispCnt = disp;
    REG_DISPCNT = disp;
    REG_BLDCNT = 0;
    REG_BLDY = 0;
    REG_WININ = 0;
    REG_WINOUT = 0;
}

static void DebugMenu_PaintOptions(void)
{
    u8 count = DebugMenu_OptionCount();
    u8 row;
    u8 idx;
    u8 y = 0;

    DebugMenu_ClearTextMap();
    DebugMenu_ClampScroll();

    for (row = 0; row < DBG_VISIBLE_ROWS; row++)
    {
        idx = (u8)(gDebugMenuScroll + row);
        if (idx >= count)
            break;

        if (gDebugMenuScreen == DBG_SCR_WARP)
            DebugMenu_PutSelected(DEBUG_WARP_TABLE[idx].name, y,
                                  idx == gDebugMenuCursor);
        else if (idx == DBG_OPT_SAVE)
            DebugMenu_PutSelected(sOptSave, y, idx == gDebugMenuCursor);
        else
            DebugMenu_PutSelected(sOptWarp, y, idx == gDebugMenuCursor);
        y++;
    }
}

static void DebugMenu_Repaint(u8 state)
{
    /* Clearing and refilling the tilemap takes long enough to straddle the
     * scanlines it feeds, so the repaint has to sit inside VBlank or the text
     * tears in and out. */
    VBlankIntrWait();
    DebugMenu_PrepareSurface();

    if (state == DBG_TEXT_SAVED)
    {
        DebugMenu_ClearTextMap();
        DebugMenu_PutLine(sStatusSaved, DBG_TEXT_X, 0);
    }
    else if (state == DBG_TEXT_SAVING)
    {
        DebugMenu_ClearTextMap();
        DebugMenu_PutLine(sStatusSaving, DBG_TEXT_X, 0);
    }
    else
    {
        DebugMenu_PaintOptions();
    }

    gSoftTextDirty = 0;
    gDebugTextActive = 0;
    gDebugMenuTextState = state;
}

static void DebugMenu_Present(void)
{
    u8 want = (u8)(gDebugMenuStatusTimer != 0 ? DBG_TEXT_SAVED : DBG_TEXT_OPTION);

    DebugMenu_SilenceVblankMaps();
    if (gDebugMenuTextState != want)
        DebugMenu_Repaint(want);
    DebugMenu_ApplyRegs();
}

static void DebugMenu_Begin(void)
{
    DBG_LOG("DBG begin");
    gDebugMenuCursor = 0;
    gDebugMenuStatusTimer = 0;
    gDebugMenuScreen = DBG_SCR_ROOT;
    gDebugMenuScroll = 0;
    gDebugMenuTextState = DBG_TEXT_NONE;
    DebugMenu_Snapshot();
#if DEBUG_MENU_LOG
    NoCashGBAPrintf("DBG bg0=%x", gDebugMenuTextBg0Cnt);
#endif
    gDebugMenuActive = DBG_MENU;
    DebugMenu_Present();
}

static void DebugMenu_Close(void)
{
    DBG_LOG("DBG close");
    gSoftTextDirty = 0;
    gDebugTextActive = 0;
    DebugMenu_RestoreRegs();
    gDebugMenuActive = DBG_CLOSED;
    gDebugMenuStatusTimer = 0;
    gDebugMenuScreen = DBG_SCR_ROOT;
    gDebugMenuScroll = 0;
}

static void DebugMenu_ForceQuietClose(void)
{
    if (gDebugMenuActive == DBG_MENU)
        DebugMenu_RestoreRegs();
    gSoftTextDirty = 0;
    gDebugTextActive = 0;
    gDebugMenuActive = DBG_CLOSED;
    gDebugMenuStatusTimer = 0;
    gDebugMenuScreen = DBG_SCR_ROOT;
    gDebugMenuScroll = 0;
}

static void DebugMenu_DoSave(void)
{
    /* END SAVE path: select slot → meta flush → EEPROM write.
     * SaveCommitPrep @ 0x56420 is player-FSM only (needs object+0x54). */
    /* The EEPROM commit blocks for ~20 frames, so put a notice up first —
     * otherwise pressing A just looks like the menu locked up. */
    DebugMenu_Repaint(DBG_TEXT_SAVING);

    gSaveBusy = 0;
    SetSaveSlot(gSaveSlot);
    FlushSaveMeta();
    VBlankIntrWait();
    WriteSave();
    VBlankIntrWait();
    gDebugMenuStatusTimer = DEBUG_STATUS_FRAMES;
    DBG_LOG("DBG saved");
}

static void DebugMenu_DoWarp(u32 modeId)
{
#if DEBUG_MENU_LOG
    NoCashGBAPrintf("DBG warp mode=%u", (unsigned)modeId);
#endif
    /* Hand the field back before changing gMode — SetMode/map prep assume a
     * live overworld surface, not the overlay mirrors. */
    DebugMenu_Close();
    QueueMode((u8)modeId);
}

static void DebugMenu_Activate(void)
{
    if (gDebugMenuScreen == DBG_SCR_WARP)
    {
        DebugMenu_DoWarp(DEBUG_WARP_TABLE[gDebugMenuCursor].modeId);
        return;
    }

    if (gDebugMenuCursor == DBG_OPT_SAVE)
    {
        DebugMenu_DoSave();
        return;
    }

    /* Enter warp submenu. */
    gDebugMenuScreen = DBG_SCR_WARP;
    gDebugMenuCursor = 0;
    gDebugMenuScroll = 0;
    gDebugMenuTextState = DBG_TEXT_NONE;
}

static void DebugMenu_UpdateMenu(void)
{
    u16 pressed = KeysPressed();

    if (gDebugMenuStatusTimer != 0)
    {
        gDebugMenuStatusTimer--;
        if (gDebugMenuStatusTimer == 0
            || (pressed & (A_BUTTON | B_BUTTON | START_BUTTON)))
            gDebugMenuStatusTimer = 0;
        DebugMenu_Present();
        return;
    }

    if (pressed & START_BUTTON)
    {
        DebugMenu_Close();
        return;
    }

    if (pressed & B_BUTTON)
    {
        if (gDebugMenuScreen == DBG_SCR_WARP)
        {
            gDebugMenuScreen = DBG_SCR_ROOT;
            gDebugMenuCursor = DBG_OPT_WARP;
            gDebugMenuScroll = 0;
            gDebugMenuTextState = DBG_TEXT_NONE;
        }
        else
        {
            DebugMenu_Close();
            return;
        }
    }

    if (pressed & DPAD_UP)
        DebugMenu_MoveCursor(-1);
    if (pressed & DPAD_DOWN)
        DebugMenu_MoveCursor(1);

    if (pressed & A_BUTTON)
        DebugMenu_Activate();

    DebugMenu_Present();
}

APPEND_TEXT bool8 DebugMenu_IsBlocking(void)
{
    DebugMenu_EnsureInit();
    return gRuntimeConfig.debug_menu
        && IsOverworldFieldMode()
        && gDebugMenuActive == DBG_MENU;
}

APPEND_TEXT bool8 DebugMenu_OnOverworldFrame(void)
{
    DebugMenu_EnsureInit();

    if (!gRuntimeConfig.debug_menu)
    {
        if (gDebugMenuActive != DBG_CLOSED)
            DebugMenu_ForceQuietClose();
        return FALSE;
    }

    if (!IsOverworldFieldMode() || gStatusMenuOpen != 0)
    {
        if (gDebugMenuActive != DBG_CLOSED)
            DebugMenu_ForceQuietClose();
        return FALSE;
    }

    if (gDebugMenuActive == DBG_MENU)
    {
        DebugMenu_UpdateMenu();
        DebugMenu_StorePrevKeys();
        return TRUE;
    }

    if ((KeysPressed() & START_BUTTON) != 0)
        DebugMenu_Begin();

    DebugMenu_StorePrevKeys();
    return gDebugMenuActive != DBG_CLOSED;
}

/* JT enters with mov pc — must not return. Skip body while menu owns the frame. */
APPEND_TEXT __attribute__((naked)) void OverworldMainFrame__Replacement(void)
{
    __asm__ volatile(
        "push {lr}\n"
        "bl DebugMenu_IsBlocking\n"
        "cmp r0, #0\n"
        "bne 1f\n"
        "pop {r0}\n"
        "mov lr, r0\n"
        "ldr r3, =0x0801A1A1\n"
        "bl 2f\n"
        "ldr r3, =0x08028A6D\n"
        "bl 2f\n"
        "ldr r3, =0x0800D619\n"
        "bx r3\n"
        "1:\n"
        "pop {r0}\n"
        "ldr r3, =0x0800D66B\n"
        "bx r3\n"
        "2:\n"
        "bx r3\n"
        ".pool\n");
}

/* Frame tail @ 0xD66A (2-mod-4 veneer). Skip cameras/draw while menu open. */
APPEND_TEXT __attribute__((naked)) void OverworldFrameTail__Replacement(void)
{
    __asm__ volatile(
        "push {lr}\n"
        "bl DebugMenu_IsBlocking\n"
        "cmp r0, #0\n"
        "bne 1f\n"
        "movs r0, #0\n"
        "ldr r3, =0x08010E59\n"
        "bl 3f\n"
        "movs r0, #0\n"
        "ldr r3, =0x08008F51\n"
        "bl 3f\n"
        "bl DebugMenu_OnOverworldFrame\n"
        "pop {r0}\n"
        "mov lr, r0\n"
        "ldr r3, =0x0800D677\n"
        "bx r3\n"
        "1:\n"
        /* Body skipped — r4 unset; avoid D67E path that loads [r4]. */
        "bl DebugMenu_OnOverworldFrame\n"
        "pop {r0}\n"
        "mov lr, r0\n"
        "ldr r3, =0x0800D6CF\n"
        "bx r3\n"
        "3:\n"
        "bx r3\n"
        ".pool\n");
}

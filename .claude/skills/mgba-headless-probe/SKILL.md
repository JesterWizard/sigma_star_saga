---
name: mgba-headless-probe
description: Build and run a headless mGBA probe against sigma_star_saga.gba to inspect live emulator state (RAM, VRAM, registers, framebuffer) instead of guessing from disassembly alone. Use whenever a bug needs runtime confirmation of register values, tilemap contents, mode transitions, or whether a hook actually executes. Also covers loading the user's real mGBA savestates (`.ss*` files) — a separate, higher-effort core build is required for that; don't discover this mid-task.
---

# mGBA headless probe

There are **two separate mGBA builds** available locally, for two different
jobs. Picking the wrong one wastes hours (it did, once already). Decide
which you need before writing any probe code:

| Need | Build | Why |
|---|---|---|
| Fresh-boot probing, no savestate involved | `/home/username/tools/mgba` (0.9.3, prebuilt headers+libs) | Simple flags, fast to compile against |
| Loading a savestate the user saved from their **real mGBA desktop app** | `/home/username/Github/mgba-orchestral` (source checkout + build, ~0.11) | The desktop app is a newer mGBA version; 0.9.3 rejects its savestate format outright |

If a task might need a user-provided savestate later, just start with the
`mgba-orchestral` build — there's no real downside, and it avoids a
mid-task toolchain swap.

## Build 1: quick fresh-boot probes (`/home/username/tools/mgba`)

```bash
MGBA_INC=/home/username/tools/mgba/usr/include
MGBA_LIB=/home/username/tools/mgba/usr/lib/x86_64-linux-gnu

gcc -O2 -o /tmp/probe probe.c \
  -I$MGBA_INC -L$MGBA_LIB \
  -Wl,-rpath-link,$MGBA_LIB -Wl,-rpath,$MGBA_LIB -lmgba

LD_LIBRARY_PATH=$MGBA_LIB timeout 600 /tmp/probe sigma_star_saga.gba <sav> 2>&1 | grep -v '^GBA '
```

`grep -v '^GBA '` strips BIOS/IO stub noise mGBA logs to stderr merged with
stdout.

## Build 2: savestate-capable probes (`mgba-orchestral`)

The `struct mCore` ABI depends on which `-D` feature flags the library was
built with (`ENABLE_VFS`, `ENABLE_DEBUGGERS`, `M_CORE_GBA`, …) — get these
wrong and `mCoreFind` returns a pointer whose function-pointer fields are
all `NULL` (looks fine, segfaults on the first call). Don't guess the
flags; read them from the library's own build:

```bash
cat /home/username/Github/mgba-orchestral/build/CMakeFiles/mgba.dir/flags.make
```

Copy the `C_DEFINES` line's `-D...` flags verbatim into your `gcc` command,
plus these three include paths (the middle one is a CMake-generated header
directory, easy to miss):

```bash
MGBA2_SRC=/home/username/Github/mgba-orchestral
MGBA2_LIB=/home/username/Github/mgba-orchestral/build

gcc -O0 -g \
  -DENABLE_DEBUGGERS -DENABLE_DIRECTORIES -DENABLE_GDB_STUB -DENABLE_SCRIPTING -DENABLE_VFS -DENABLE_VFS_FD \
  -DHAVE_CRC32 -DHAVE_FREELOCALE -DHAVE_FUTIMENS -DHAVE_FUTIMES -DHAVE_LOCALE -DHAVE_LOCALTIME_R -DHAVE_NEWLOCALE \
  -DHAVE_PTHREAD_CREATE -DHAVE_PTHREAD_SETNAME_NP -DHAVE_REALPATH -DHAVE_SETLOCALE -DHAVE_STRDUP -DHAVE_STRNDUP \
  -DHAVE_USELOCALE -DHAVE_VASPRINTF -DM_CORE_GB -DM_CORE_GBA -DUSE_LZMA -DUSE_MINIZIP -DUSE_PNG -DUSE_PTHREADS \
  -DUSE_SQLITE3 -DUSE_ZLIB -D_GNU_SOURCE \
  -I$MGBA2_SRC/include -I$MGBA2_LIB/include -I$MGBA2_SRC/src \
  -o /tmp/probe probe.c \
  -L$MGBA2_LIB -Wl,-rpath-link,$MGBA2_LIB -Wl,-rpath,$MGBA2_LIB -lmgba -lpthread

LD_LIBRARY_PATH=$MGBA2_LIB timeout 600 /tmp/probe sigma_star_saga.gba <sav> [<ss>] 2>&1 | grep -v '^GBA \|^\[LOG'
```

If `flags.make` ever moves or the build is reconfigured, re-read it fresh —
don't reuse a cached flag list across sessions without checking it still
matches the on-disk build.

**Sanity check before trusting the build**: after `mCoreFind`, print
`core->init` and `core->deinit` — if either is `NULL`, the ABI is
mismatched (wrong flags) and every subsequent call is landing on garbage
offsets, not a real crash you can debug normally.

```c
struct mCore* core = mCoreFind(argv[1]);
fprintf(stderr, "core->init=%p\n", (void*)core->init);  /* must be non-NULL */
```

## Loading a user-provided savestate (`.ss0`-`.ss9`)

```c
struct VFile* state = VFileOpen(ssPath, O_RDWR);   /* O_RDWR, same rule as .sav */
bool ok = mCoreLoadStateNamed(core, state, SAVESTATE_SAVEDATA | SAVESTATE_RTC);
state->close(state);
if (!ok) { /* see failure triage below */ }
```

Do this **after** `core->loadROM` + `core->loadSave` +
`GBASavedataForceType` + `core->reset()` — same ordering as a fresh boot,
the state load then overwrites the reset state.

### Two failure modes, and how to tell them apart

Install a verbose logger before attempting the load — the default logger
silently swallows the exact rejection reason:

```c
static void myLog(struct mLogger* l, int cat, enum mLogLevel lvl, const char* fmt, va_list args) {
    vfprintf(stderr, fmt, args); fprintf(stderr, "\n");
}
static struct mLogger logger = { .log = myLog };
mLogSetDefaultLogger(&logger);   /* call before mCoreFind */
```

1. **`Invalid or too new savestate: expected 0100000X, got 0100000Y`** — the
   probe's mGBA library is *older* than the app that saved the state.
   `mCoreLoadStateNamed` returns `false` outright; nothing loads. Fix: use
   the `mgba-orchestral` build (newer), not the prebuilt 0.9.3 one.
2. **`Savestate created using a different version of the BIOS: expected
   AAAAAAAA, got BBBBBBBB`** — the load *succeeds* (`mCoreLoadStateNamed`
   returns `true`) but the CPU then executes garbage: "Jumped to invalid
   address", "Illegal opcode" spam, dead framebuffer. This is not a fatal
   rejection, it's a silent corruption — don't treat `ok == true` alone as
   "safe to trust the resulting state."
   - Check the user's real mGBA config for which BIOS they use:
     `grep bios ~/.config/mgba/config.ini`. An **empty** `gba.bios=` line
     means they're on mGBA's built-in HLE BIOS — do **not** load a real
     BIOS file (`core->loadBIOS`) in the probe, it will actively make this
     worse (different checksum, same corruption). Leave BIOS unset to match
     HLE.
   - If they *do* have a `gba.bios=/path/to/file.bin` line, load that exact
     file, not any other legitimate GBA BIOS dump — the checksum has to
     match bit-for-bit, "a real BIOS" isn't good enough.
   - Confirmed-clean load looks like real `DMA`/`SWI` log lines with sane
     addresses (`Starting DMA 3 0x03001F70 -> 0x07000000`) and zero
     "invalid address" / "illegal opcode" lines in the first few frames.

### ROM/state mismatch is usually a red herring

The state's PNG chunks (mGBA `.ss*` files are PNG with custom chunks —
`gbAs` holds the zlib-compressed core state) embed the ROM's title, game
code, and a CRC32 in the first ~24 decompressed bytes. If you suspect the
probe's ROM doesn't match what the state expects, check this directly
instead of guessing:

```python
import struct, zlib
data = open('state.ss1', 'rb').read()
idx = 8
while idx < len(data):
    length = struct.unpack('>I', data[idx:idx+4])[0]
    tag = data[idx+4:idx+8]
    if tag == b'gbAs':
        payload = zlib.decompress(data[idx+8:idx+8+length])
        embedded_crc = struct.unpack('<I', payload[8:12])[0]  # offset varies; scan for it
        break
    idx += 8 + length + 4
rom_crc = zlib.crc32(open('sigma_star_saga.gba', 'rb').read()) & 0xffffffff
print(hex(embedded_crc), hex(rom_crc))
```

A rebuild that only changes a handful of debug-menu bytes (not a symbol
layout change) usually keeps the same ROM otherwise-identical enough that
a savestate taken minutes earlier still matches — check the CRC before
assuming you need the *exact* build the user played, which would make
savestates useless for iterating on a fix.

## Minimal probe skeleton

```c
#include <mgba/core/core.h>
#include <mgba-util/vfs.h>
#include <mgba/internal/gba/gba.h>
#include <mgba/internal/gba/savedata.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    setvbuf(stdout, NULL, _IONBF, 0);  /* IMPORTANT: see gotcha below */
    struct mCore* core = mCoreFind(argv[1]);
    core->init(core);
    mCoreInitConfig(core, NULL);
    uint32_t* fb = calloc(240 * 160, sizeof(uint32_t));
    core->setVideoBuffer(core, (void*)fb, 240);
    core->loadROM(core, VFileOpen(argv[1], O_RDONLY));
    if (argc > 2 && strcmp(argv[2], "-"))
        core->loadSave(core, VFileOpen(argv[2], O_RDWR));  /* must be RDWR or load crashes */
    /* GBA_SAVEDATA_EEPROM512 on the mgba-orchestral build; SAVEDATA_EEPROM512 on the 0.9.3 one */
    GBASavedataForceType(&((struct GBA*)core->board)->memory.savedata, GBA_SAVEDATA_EEPROM512);
    core->reset(core);

    for (int f = 0; f < 30000; f++) {
        core->setKeys(core, /* bitmask, see below */ 0);
        core->runFrame(core);
    }
    printf("gMode=%02X\n", core->busRead8(core, 0x03001630));
    return 0;
}
```

Read/write memory with `core->busRead8/16/32(core, addr)` and
`core->busWrite8/16/32(core, addr, val)` — these work on live GBA address
space (IWRAM `0x0300xxxx`, EWRAM `0x02xxxxxx`, VRAM `0x06xxxxxx`, PALRAM
`0x05xxxxxx`, ROM `0x08xxxxxx`).

Key bitmask (GBA KEYINPUT, active-low internally but `setKeys` takes active-high):
`A=0x001 B=0x002 SELECT=0x004 START=0x008 RIGHT=0x010 LEFT=0x020 UP=0x040 DOWN=0x080 R=0x100 L=0x200`.

## Proven boot sequences (stop rediscovering these)

**Fresh boot, no save file** (splash → title → story intro → first flight
stage), confirmed working from `core->reset()`:

```c
/* ~1131 frames: mash through splash/title/intro dialogue-skip cutscene */
for (int frame = 0; frame < 1131; frame++) {
    uint32_t keys = 0;
    int ph = frame % 40;
    if (ph < 4) keys = 0x001;              /* A */
    else if (ph >= 10 && ph < 14) keys = 0x008;  /* START */
    else if (ph >= 20 && ph < 30) keys = 0x204;  /* SELECT+L (flight-skip cheat, pre-flight only) */
    core->setKeys(core, keys);
    core->runFrame(core);
}
/* then ~122 taps of plain A (3 frames pressed / 20 released) to advance
 * intro dialogue text -> lands in gMode=0x84, a real flight stage,
 * ship+HUD visible */
for (int step = 0; step < 122; step++) {
    for (int i = 0; i < 3; i++) { core->setKeys(core, 0x001); core->runFrame(core); }
    for (int i = 0; i < 20; i++) { core->setKeys(core, 0); core->runFrame(core); }
}
```

**Real save file → File Select → gameplay** (confirmed working against a
save with actual progress):

```c
/* same splash/title mash as above, ~2371 frames of A this time (title
 * takes longer to reach with a save present) */
for (int frame = 0; frame < 2371; frame++) {
    uint32_t keys = 0;
    int ph = frame % 70;
    if (ph < 10) keys = 0x001;
    core->setKeys(core, keys);
    core->runFrame(core);
}
/* then hold START (10-on/60-off cycles) UNTIL gMode == 0x0E (File Select) --
 * poll for it, don't hardcode a frame count, it drifts between runs */
uint8_t m = 0;
for (int frame = 0; frame < 2000; frame++) {
    int cyc = frame % 70;
    core->setKeys(core, (cyc < 10) ? 0x008 : 0);
    core->runFrame(core);
    m = core->busRead8(core, 0x03001630);
    if (m == 0x0E) break;
}
/* CRITICAL: wait ~180 frames with no input before pressing A. The mode
 * flips to 0x0E before the File Select art/cursor has actually faded in;
 * an A press during that window is swallowed with no visible effect and
 * looks identical to "menu doesn't respond." Screenshot to confirm the
 * cursor/menu text is actually rendered before trusting gMode alone. */
core->setKeys(core, 0);
for (int i = 0; i < 180; i++) core->runFrame(core);
/* DOWN moves between save slots; the default cursor position varies by
 * save content, so screenshot first rather than assuming slot 0 works */
for (int i = 0; i < 3; i++) { core->setKeys(core, 0x001); core->runFrame(core); }  /* A: pick slot */
for (int i = 0; i < 10; i++) { core->setKeys(core, 0); core->runFrame(core); }
```

This save-load path also transiently visits a "???"-grid item-summary
screen (`gMode=0x8F`) that looks stuck (no button advances it — confirmed A,
B, SELECT, all directions; only re-entering from File Select via SELECT
backs out of it). If you land there, you likely mistimed the File Select
step above (pressed A on the wrong screen/frame) — back up and redo it with
a screenshot checkpoint, don't try to force `0x8F` forward.

## Gotchas that cost real time

- **Unbuffered stdout is mandatory.** A probe that runs 100k+ `runFrame()`
  calls without hitting a flush point can look "hung" and get killed by
  `timeout` with zero output. Always `setvbuf(stdout, NULL, _IONBF, 0)`
  first, or your `printf`s vanish along with the exit code.
- **`loadSave` needs `O_RDWR`, not `O_RDONLY`**, even for a read-only probe —
  mGBA's save-type autodetect writes back and segfaults on a read-only
  VFile. Same rule applies to `.ss*` savestate files passed to
  `mCoreLoadStateNamed`.
- **Never point a probe at the user's real save or savestate file.**
  `loadSave`/`mCoreLoadStateNamed` open read-write and gameplay frames write
  back — running frames against the real file mutates it permanently and
  git may not have it tracked to restore from. Always `cp` to a scratch path
  first and probe the copy.
- **Scripted button-mashing reaches menus, not gameplay.** A generic
  "press A sometimes, START sometimes" loop gets stuck oscillating between
  two modes (e.g. title↔attract, or game-over↔continue) indefinitely, or
  overshoots a menu that needed a screenshot-confirmed pause first (see File
  Select above). If a proven sequence exists above, use it verbatim before
  improvising a new one.
- **A warp/teleport debug feature may not run the destination map's spawn
  init.** Don't assume `gMode` sticking at the target value means the map is
  live — check that a player-position or player-index symbol actually
  changes when you feed movement input. If it doesn't move, the walk-update
  hook for that mode isn't running and nothing gated behind it (encounters,
  triggers) will fire, regardless of how correct your patch is.
- **A `gMode` value alone doesn't prove a screen is visually ready.** State
  can flip a frame or more before the corresponding art/menu has actually
  drawn (see the File Select 180-frame wait above). When a scripted input
  seems to do nothing, screenshot before concluding the input is wrong —
  it's just as likely you're a few frames early.
- **Frame counts drift between separate probe runs/builds** even with
  identical input scripts, because upstream boot timing (title animation
  length, etc.) isn't perfectly deterministic across recompiles. Don't
  hardcode "the death happens at frame 2295" across two different probe
  binaries and expect it to land on the same frame in both — re-derive the
  frame number (or better, poll for a state change) in the same run you're
  using it, not from a previous run's printed number.

## Screenshot an unknown mode

```c
static void shot(const char* path, uint32_t* fb) {
    FILE* f = fopen(path, "wb");
    fprintf(f, "P6\n240 160\n255\n");
    for (int i = 0; i < 240 * 160; i++) {
        uint32_t p = fb[i];
        unsigned char rgb[3] = {(p>>16)&0xFF, (p>>8)&0xFF, p&0xFF};
        fwrite(rgb, 1, 3, f);
    }
    fclose(f);
}
```

Convert the PPM to PNG for the Read tool with a small inline Python/zlib
script (no ImageMagick dependency needed) — build an IHDR/IDAT/IEND chunk
manually with `zlib.compress` if `pnmtopng` isn't installed. Then actually
`Read` the PNG — don't rely on pixel-sum heuristics as a proxy for "did the
thing I'm watching visibly change" (see the "watch for a visible-effect
correlate" section in gba-causal-trace-hunt for why a cheap numeric summary
of a screen region can silently measure the wrong thing, like an idle
shimmer animation instead of real damage).

## Finding symbol addresses

```bash
arm-none-eabi-nm sigma_star_saga.elf | grep -i <SymbolName>
```

Don't hand-guess EWRAM debug-menu symbol offsets — they shift between builds.
Always resolve via `nm` on the just-built `.elf` before writing a probe that
pokes them.

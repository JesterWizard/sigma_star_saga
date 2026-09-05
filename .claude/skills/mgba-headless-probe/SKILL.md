---
name: mgba-headless-probe
description: Build and run a headless mGBA probe against sigma_star_saga.gba to inspect live emulator state (RAM, VRAM, registers, framebuffer) instead of guessing from disassembly alone. Use whenever a bug needs runtime confirmation of register values, tilemap contents, mode transitions, or whether a hook actually executes.
---

# mGBA headless probe

The repo already has a compiled mGBA build with dev headers/libs unpacked
locally (no system package). Reuse this path instead of rediscovering it.

## Toolchain locations

```bash
MGBA_INC=/home/username/tools/mgba/usr/include
MGBA_LIB=/home/username/tools/mgba/usr/lib/x86_64-linux-gnu
```

Compile:

```bash
gcc -O2 -o /tmp/probe probe.c \
  -I$MGBA_INC -L$MGBA_LIB \
  -Wl,-rpath-link,$MGBA_LIB -Wl,-rpath,$MGBA_LIB -lmgba
```

Run (needs `LD_LIBRARY_PATH` even with `-rpath` baked in, for some libav* transitive deps):

```bash
LD_LIBRARY_PATH=$MGBA_LIB timeout 600 /tmp/probe sigma_star_saga.gba <sav> 2>&1 | grep -v '^GBA '
```

`grep -v '^GBA '` strips BIOS/IO stub noise mGBA logs to stderr merged with stdout.

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
    GBASavedataForceType(&((struct GBA*)core->board)->memory.savedata, SAVEDATA_EEPROM512);
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

## Gotchas that cost real time

- **Unbuffered stdout is mandatory.** A probe that runs 100k+ `runFrame()`
  calls without hitting a flush point can look "hung" and get killed by
  `timeout` with zero output. Always `setvbuf(stdout, NULL, _IONBF, 0)`
  first, or your `printf`s vanish along with the exit code.
- **`loadSave` needs `O_RDWR`, not `O_RDONLY`**, even for a read-only probe —
  mGBA's save-type autodetect writes back and segfaults on a read-only
  VFile.
- **Never point a probe at the user's real save file.** `loadSave` opens it
  read-write and gameplay frames write back to it — running frames against
  `sigma_star_saga.sav` mutates it permanently and git won't have it tracked
  to restore from. Always `cp` to a scratch path first and probe the copy.
  If you already ran against the real file, diff/restore from a backup
  immediately rather than continuing.
- **mCoreLoadStateNamed / mCoreSaveStateNamed are unreliable across probe
  binaries** in this setup — savestates saved by one probe often fail to
  reload in another (`loadState failed`). Don't build a checkpoint-and-resume
  strategy around them without first proving load works with the exact
  core-init sequence you'll reuse.
- **Scripted button-mashing reaches menus, not gameplay.** A generic
  "press A sometimes, START sometimes" loop gets stuck oscillating between
  two modes (e.g. title↔attract, or game-over↔continue) indefinitely. If you
  need real progress, screenshot the unknown mode first (see snippet below)
  and script the *specific* input sequence for that menu rather than
  broadening the RNG of keys pressed.
- **A warp/teleport debug feature may not run the destination map's spawn
  init.** Don't assume `gMode` sticking at the target value means the map is
  live — check that a player-position or player-index symbol actually
  changes when you feed movement input. If it doesn't move, the walk-update
  hook for that mode isn't running and nothing gated behind it (encounters,
  triggers) will fire, regardless of how correct your patch is.

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
script (no ImageMagick dependency needed) — see the pattern of building an
IHDR/IDAT/IEND chunk manually with `zlib.compress` if `pnmtopng` isn't
installed.

## Finding symbol addresses

```bash
arm-none-eabi-nm sigma_star_saga.elf | grep -i <SymbolName>
```

Don't hand-guess EWRAM debug-menu symbol offsets — they shift between builds.
Always resolve via `nm` on the just-built `.elf` before writing a probe that
pokes them.

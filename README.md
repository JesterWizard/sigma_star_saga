# Sigma Star Saga

Decompilation scaffold for *Sigma Star Saga* (GBA), structured after [pret/pokeemerald](https://github.com/pret/pokeemerald).

## Status

Early scaffold: the ROM is rebuilt by `.incbin`-ing `baserom.gba` so `make compare` can pass before any code is matched. Peel ranges out of `asm/rom.s` into real asm/C as work progresses.

## Quick start

See [INSTALL.md](INSTALL.md).

```bash
# after placing baserom.gba
make compare
```

## Layout

| Path | Role |
|------|------|
| `asm/` | Hand-written / disassembled ARM/Thumb |
| `src/` | Decompiled C (wired into the Makefile when ready) |
| `include/`, `include/gba/` | Headers (GBA register/types from pret style) |
| `data/`, `graphics/`, `sound/` | Extracted assets (empty for now) |
| `ld_script.ld` | Memory map / object order |
| `tools/` | pret tools + `agbcc` install location |
| `rom.sha1` | Matching checksum for `make compare` |

## Matching workflow

1. Disassemble / analyze `baserom.gba` (Ghidra, etc.).
2. Replace a range in `asm/rom.s` with a real object in `asm/` or `src/`.
3. Update `ld_script.ld` and the Makefile source lists.
4. `make compare` — keep the SHA1 green.

Target ROM: **USA/Europe**, SHA1 `132222e0c0ddacb6281cc2eb4489ed6c8719dac2`.

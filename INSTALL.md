# Installation

## Dependencies (Debian/Ubuntu/WSL)

```bash
sudo apt update
sudo apt install -y build-essential git python3 libpng-dev binutils-arm-none-eabi
```

Optional: install [devkitPro](https://devkitpro.org/wiki/Getting_Started) `gba-dev` and ensure `DEVKITARM` is set. The Makefile prefers `arm-none-eabi-*` on `PATH`.

## Baserom

Place a clean **Sigma Star Saga (USA/Europe)** dump in the repo root as:

```text
baserom.gba
```

Confirm header fields (and update the Makefile if needed):

```bash
python3 scripts/print_header.py
```

Expected No-Intro SHA1 for the target ROM:

```text
132222e0c0ddacb6281cc2eb4489ed6c8719dac2
```

## Build tools (optional at scaffold stage)

```bash
make tools
```

This sparse-clones [pret/pokeemerald](https://github.com/pret/pokeemerald) and builds `gbafix`, `scaninc`, `preproc`, `ramscrgen`, and `gbagfx`.

## Install agbcc (needed once you compile matching C)

```bash
git clone https://github.com/pret/agbcc ../agbcc
cd ../agbcc
./build.sh
./install.sh ../sigma_star_saga
cd ../sigma_star_saga
```

## Build / compare

```bash
make            # builds sigma_star_saga.gba from baserom.gba
make compare    # builds and checks rom.sha1
```

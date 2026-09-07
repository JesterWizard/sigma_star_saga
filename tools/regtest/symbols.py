"""Resolve EWRAM/IWRAM symbol addresses from the just-built ELF.

Debug-menu and other custom EWRAM symbols shift address between builds
(bump-allocated in asm/ram_map_ewram.s). Never hardcode one of these
addresses in a test case -- always look it up here, fresh, against the ELF
the test is about to run. See tools/.claude/skills/mgba-headless-probe's
"Finding symbol addresses" section for why this matters.
"""
from __future__ import annotations

import functools
import re
import subprocess
from pathlib import Path

_NM_LINE = re.compile(r"^([0-9a-fA-F]{8})\s+\S\s+(\S+)$")


@functools.lru_cache(maxsize=1)
def _all_symbols(elf_path: str) -> dict[str, int]:
    out = subprocess.run(
        ["arm-none-eabi-nm", elf_path],
        check=True,
        capture_output=True,
        text=True,
    ).stdout
    symbols: dict[str, int] = {}
    for line in out.splitlines():
        m = _NM_LINE.match(line.strip())
        if m:
            symbols[m.group(2)] = int(m.group(1), 16)
    return symbols


class SymbolTable:
    """Symbol lookups scoped to one ELF file, cached for the process lifetime."""

    def __init__(self, elf_path: Path | str):
        self.elf_path = str(elf_path)
        if not Path(self.elf_path).exists():
            raise FileNotFoundError(
                f"{self.elf_path} not found -- build the ROM first (make)"
            )

    def addr(self, name: str) -> int:
        symbols = _all_symbols(self.elf_path)
        try:
            return symbols[name]
        except KeyError as exc:
            raise KeyError(
                f"symbol '{name}' not found in {self.elf_path} -- "
                "renamed, removed, or not yet built?"
            ) from exc

    def has(self, name: str) -> bool:
        return name in _all_symbols(self.elf_path)

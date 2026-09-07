"""TestCase protocol shared by every module under tools/regtest/cases/."""
from __future__ import annotations

from dataclasses import dataclass
from typing import Callable

from .harness import Harness
from .symbols import SymbolTable


@dataclass
class TestCase:
    name: str
    # fn(h, symbols) -> None; raise AssertionError (or let one propagate) to fail.
    fn: Callable[[Harness, SymbolTable], None]
    description: str = ""

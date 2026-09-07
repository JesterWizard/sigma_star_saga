#!/usr/bin/env python3
"""Build-time regression test suite for sigma_star_saga.

Boots the just-built ROM under headless mGBA and asserts gameplay invariants
that a bad hook (bad memory write, missing mode gate, wrong offset) can
silently break without any compiler error -- see documentation/debug-menu.md
"Ship picker -- removed, incident record" for the bug this suite exists to
catch category-of.

Usage:
    python3 tools/regtest/run_regtests.py [--mgba-lib DIR] [-k SUBSTRING]

Requires:
    - sigma_star_saga.gba and sigma_star_saga.elf already built (`make`)
    - tools/regtest_harness already built (`make regtest-harness`, or this
      script will try to build it itself if libmgba's dev headers/libs are
      found at the default path used by .claude/skills/mgba-headless-probe)
"""
from __future__ import annotations

import argparse
import importlib
import pkgutil
import subprocess
import sys
import traceback
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
ROM_PATH = REPO_ROOT / "sigma_star_saga.gba"
ELF_PATH = REPO_ROOT / "sigma_star_saga.elf"
HARNESS_SRC = REPO_ROOT / "tools" / "regtest_harness.c"
HARNESS_BIN = REPO_ROOT / "tools" / "regtest_harness"

DEFAULT_MGBA_INC = Path("/home/username/tools/mgba/usr/include")
DEFAULT_MGBA_LIB = Path("/home/username/tools/mgba/usr/lib/x86_64-linux-gnu")


def _ensure_harness_built(mgba_inc: Path, mgba_lib: Path) -> None:
    if HARNESS_BIN.exists() and HARNESS_BIN.stat().st_mtime >= HARNESS_SRC.stat().st_mtime:
        return
    print(f"[regtest] building {HARNESS_BIN} ...")
    subprocess.run(
        [
            "gcc", "-O2", "-o", str(HARNESS_BIN), str(HARNESS_SRC),
            f"-I{mgba_inc}", f"-L{mgba_lib}",
            f"-Wl,-rpath-link,{mgba_lib}", f"-Wl,-rpath,{mgba_lib}", "-lmgba",
        ],
        check=True,
    )


def _discover_cases():
    from . import cases as cases_pkg

    all_cases = []
    for _, modname, _ in pkgutil.iter_modules(cases_pkg.__path__):
        module = importlib.import_module(f"{cases_pkg.__name__}.{modname}")
        for case in getattr(module, "CASES", []):
            all_cases.append((modname, case))
    return all_cases


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--mgba-inc", type=Path, default=DEFAULT_MGBA_INC)
    parser.add_argument("--mgba-lib", type=Path, default=DEFAULT_MGBA_LIB)
    parser.add_argument(
        "-k", "--filter", default=None,
        help="only run cases whose 'module.name' contains this substring",
    )
    args = parser.parse_args()

    if not ROM_PATH.exists() or not ELF_PATH.exists():
        print(f"[regtest] {ROM_PATH.name} / {ELF_PATH.name} not found -- run `make` first",
              file=sys.stderr)
        return 2

    try:
        _ensure_harness_built(args.mgba_inc, args.mgba_lib)
    except subprocess.CalledProcessError as exc:
        print(f"[regtest] failed to build harness: {exc}", file=sys.stderr)
        return 2

    from .harness import Harness
    from .symbols import SymbolTable

    all_cases = _discover_cases()
    if args.filter:
        all_cases = [
            (mod, case) for mod, case in all_cases
            if args.filter in f"{mod}.{case.name}"
        ]

    if not all_cases:
        print("[regtest] no test cases matched", file=sys.stderr)
        return 2

    symbols = SymbolTable(ELF_PATH)

    passed, failed = [], []
    for modname, case in all_cases:
        full_name = f"{modname}.{case.name}"
        print(f"[regtest] {full_name} ... ", end="", flush=True)
        try:
            with Harness(mgba_lib_dir=str(args.mgba_lib)) as h:
                h.boot(ROM_PATH)
                case.fn(h, symbols)
            print("PASS")
            passed.append(full_name)
        except AssertionError as exc:
            print("FAIL")
            print(f"    {exc}")
            failed.append((full_name, str(exc)))
        except Exception:
            print("ERROR")
            traceback.print_exc()
            failed.append((full_name, "unexpected exception, see traceback above"))

    print()
    print(f"[regtest] {len(passed)} passed, {len(failed)} failed "
          f"(of {len(all_cases)})")
    if failed:
        print("[regtest] failures:")
        for name, msg in failed:
            print(f"  - {name}: {msg}")
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

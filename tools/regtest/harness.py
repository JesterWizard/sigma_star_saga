"""Python driver for tools/regtest_harness -- a scripted headless mGBA probe.

Each Harness instance owns one subprocess running the compiled probe binary
and talks to it over a line-oriented stdin/stdout protocol (see
tools/regtest_harness.c for the full command list). Test cases use this
class instead of talking to the subprocess directly.
"""
from __future__ import annotations

import os
import subprocess
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
HARNESS_BIN = REPO_ROOT / "tools" / "regtest_harness"

# GBA KEYINPUT bits, active-high as the harness expects them.
KEY_A = 0x001
KEY_B = 0x002
KEY_SELECT = 0x004
KEY_START = 0x008
KEY_RIGHT = 0x010
KEY_LEFT = 0x020
KEY_UP = 0x040
KEY_DOWN = 0x080
KEY_R = 0x100
KEY_L = 0x200


class HarnessError(RuntimeError):
    pass


class Harness:
    """One live headless-mGBA subprocess, scripted via stdin/stdout."""

    def __init__(self, mgba_lib_dir: str | None = None, timeout: float = 120.0):
        if not HARNESS_BIN.exists():
            raise FileNotFoundError(
                f"{HARNESS_BIN} not found -- build it first "
                "(see tools/regtest/README.md or `make regtest-harness`)"
            )
        env = dict(os.environ)
        if mgba_lib_dir:
            existing = env.get("LD_LIBRARY_PATH", "")
            env["LD_LIBRARY_PATH"] = (
                f"{mgba_lib_dir}:{existing}" if existing else mgba_lib_dir
            )
        self._timeout = timeout
        self._proc = subprocess.Popen(
            [str(HARNESS_BIN)],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1,
            env=env,
        )

    def _send(self, line: str) -> str:
        assert self._proc.stdin is not None and self._proc.stdout is not None
        self._proc.stdin.write(line + "\n")
        self._proc.stdin.flush()
        reply = self._proc.stdout.readline()
        if not reply:
            stderr = self._proc.stderr.read() if self._proc.stderr else ""
            raise HarnessError(
                f"harness exited unexpectedly after '{line}'\nstderr:\n{stderr}"
            )
        reply = reply.strip()
        if reply.startswith("ERR"):
            raise HarnessError(f"'{line}' -> {reply}")
        return reply

    def boot(self, rom_path: Path | str, sav_path: Path | str | None = None) -> None:
        sav = str(sav_path) if sav_path else "-"
        self._send(f"BOOT {rom_path} {sav}")

    def run(self, frames: int, keys: int = 0) -> None:
        self._send(f"RUN {frames} {keys:x}")

    def run_until8(self, addr: int, value: int, max_frames: int) -> tuple[bool, int]:
        """Run with no keys held until byte@addr == value or max_frames elapse.

        Returns (reached, frames_run).
        """
        reply = self._send(f"RUN_UNTIL8 {addr:x} {value:x} {max_frames}")
        kind, frames = reply.split()
        return kind == "REACHED", int(frames)

    def read8(self, addr: int) -> int:
        return int(self._send(f"RD8 {addr:x}").split()[1], 16)

    def read16(self, addr: int) -> int:
        return int(self._send(f"RD16 {addr:x}").split()[1], 16)

    def read32(self, addr: int) -> int:
        return int(self._send(f"RD32 {addr:x}").split()[1], 16)

    def write8(self, addr: int, value: int) -> None:
        self._send(f"WR8 {addr:x} {value:x}")

    def write16(self, addr: int, value: int) -> None:
        self._send(f"WR16 {addr:x} {value:x}")

    def write32(self, addr: int, value: int) -> None:
        self._send(f"WR32 {addr:x} {value:x}")

    def screenshot(self, path: Path | str) -> None:
        self._send(f"SHOT {path}")

    def close(self) -> None:
        if self._proc.poll() is None:
            try:
                self._send("QUIT")
            except HarnessError:
                pass
            try:
                self._proc.wait(timeout=5)
            except subprocess.TimeoutExpired:
                self._proc.kill()

    def __enter__(self) -> "Harness":
        return self

    def __exit__(self, *exc_info) -> None:
        self.close()

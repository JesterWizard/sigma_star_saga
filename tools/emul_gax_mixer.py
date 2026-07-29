#!/usr/bin/env python3
"""Host GAX mixer harness (Unicorn) — proves the standalone speech runtime path.

Boots the real ROM's GAX driver (Gax2New/Gax2Init), applies the catalog song,
runs the real GaxAttachSpeech / GaxPlayVoice from the built ROM, then drives
speech consumer ticks at 0x08056A30 with instruction budgets and memory-fault
hooks. Reproduces in-emulator hangs/corruption without a GBA emulator.

usage: tools/emul_gax_mixer.py [--voice 1] [--ticks 40] [--silence]
"""

from __future__ import annotations

import argparse
import struct
import sys
from collections import deque
from pathlib import Path

from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_PROT_ALL, UC_PROT_READ, UC_PROT_EXEC
from unicorn import UC_HOOK_CODE, UC_HOOK_INTR, UC_HOOK_MEM_INVALID
from unicorn.arm_const import (
    UC_ARM_REG_R0,
    UC_ARM_REG_R1,
    UC_ARM_REG_R2,
    UC_ARM_REG_R3,
    UC_ARM_REG_R4,
    UC_ARM_REG_R5,
    UC_ARM_REG_R6,
    UC_ARM_REG_R7,
    UC_ARM_REG_R8,
    UC_ARM_REG_R9,
    UC_ARM_REG_R10,
    UC_ARM_REG_R11,
    UC_ARM_REG_R12,
    UC_ARM_REG_R13,
    UC_ARM_REG_R14,
    UC_ARM_REG_PC,
)

REPO = Path(__file__).resolve().parents[1]
ROM_PATH = REPO / "sigma_star_saga.gba"

# ROM functions (vanilla)
GAX2_NEW = 0x08054AB5
GAX2_INIT = 0x08054F95
GAX_SONG_APPLY = 0x08054891
GAX_SPEECH_OBJ_INIT = 0x0805770D
PLAY_SFX = 0x08055C8D
CONSUMER = 0x08056A30  # veneer → probe in built ROM
MIXER_WRAPPER = 0x080569CC  # per-tick wrapper(r0 flag, r1 song state)

# Appended (custom) — from arm-none-eabi-nm sigma_star_saga.elf
GAX_ATTACH = 0x088040A0
GAX_PLAY_VOICE = 0x0880421C

# RAM symbols (nm)
G_GAX_PARAMS = 0x030008C0
G_GAX_MIXBUF = 0x03005910
G_GAX_WS_PTR = 0x0300775C
G_SPEECH_OBJ = 0x03004348
G_SPEECH_OWNER = 0x03007B74
G_MUSIC_TABLE = 0x08821664
G_VOICE_TABLE = 0x08825BC8
G_RUNTIME_CFG = 0x08800000
CFG_OFF_GAX_AUDIO = 0x14

EXIT_SENTINEL = 0xDEADBEEF
BUDGET = 4_000_000

WATCH = {
    0x0805770C: "ROM speech init",
    0x08056A30: "consumer veneer",
    0x08804328: "consumer probe",
    0x088040A0: "GaxAttachSpeech",
    0x0880421C: "GaxPlayVoice",
    0x08055C8C: "PlaySfx",
    EXIT_SENTINEL & ~1: "EXIT",
}

# QEMU/Unicorn rejects LDM with base-in-list + writeback; real ARM7TDMI runs
# it. Assist these sites by emulating the single instruction (no writeback
# when the base register is in the load list).
BAD_LDM = {0x03005E74: 0xE8B0000F}  # ldmia r0!, {r0-r3}

INSTR_LIMIT_TOTAL = 200_000_000


class Harness:
    def __init__(self, rom: bytes):
        self.uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB)
        self.rom = rom
        self.instr = 0
        self.call_instr = 0
        self.recent = deque(maxlen=48)
        self.div_log = []
        self.svc_log = []
        self.halted = False
        self._map()
        uc = self.uc
        uc.hook_add(UC_HOOK_INTR, self._on_intr)
        uc.hook_add(UC_HOOK_CODE, self._on_code)
        uc.hook_add(UC_HOOK_MEM_INVALID, self._on_mem_invalid)
        self._write_watch = None

    def watch_writes(self, lo, hi):
        from unicorn import UC_HOOK_MEM_WRITE

        self._write_watch = (lo, hi)

        def _on_write(uc, access, address, size, value, user):
            if lo <= address < hi:
                pc = uc.reg_read(UC_ARM_REG_PC)
                print(
                    f"   ## write {address:08X} <- {value:0{size*2}X} "
                    f"(size {size}) pc={pc:08X} lr={uc.reg_read(UC_ARM_REG_R14):08X}"
                )

        self.uc.hook_add(UC_HOOK_MEM_WRITE, _on_write)

    def _map(self):
        uc = self.uc
        uc.mem_map(0x00000000, 0x4000)  # BIOS stub (svc handled in intr hook)
        uc.mem_map(0x02000000, 0x40000)  # EWRAM
        uc.mem_map(0x03000000, 0x8000)  # IWRAM
        uc.mem_map(0x04000000, 0x10000)  # IO
        uc.mem_map(0x04FFF000, 0x1000)  # No$GBA debug port
        uc.mem_map(0x05000000, 0x400)
        uc.mem_map(0x06000000, 0x18000)
        uc.mem_map(0x07000000, 0x400)
        size = (len(self.rom) + 0xFFFF) & ~0xFFFF
        uc.mem_map(0x08000000, size, UC_PROT_READ | UC_PROT_EXEC)
        uc.mem_write(0x08000000, self.rom)
        uc.mem_map(0x0E000000, 0x10000)

    # -- helpers ---------------------------------------------------------
    def r32(self, addr):
        return struct.unpack("<I", self.uc.mem_read(addr, 4))[0]

    def r16(self, addr):
        return struct.unpack("<H", self.uc.mem_read(addr, 2))[0]

    def r8(self, addr):
        return self.uc.mem_read(addr, 1)[0]

    def w32(self, addr, val):
        self.uc.mem_write(addr, struct.pack("<I", val & 0xFFFFFFFF))

    def w16(self, addr, val):
        self.uc.mem_write(addr, struct.pack("<H", val & 0xFFFF))

    def _on_code(self, uc, address, size, user):
        self.instr += 1
        self.call_instr += 1
        self.recent.append(address)
        if address in BAD_LDM:
            self._assist_ldm(address)
            return
        if address in WATCH and address != (EXIT_SENTINEL & ~1):
            print(f"   -> {WATCH[address]} @ {address:08X}")
        if address == (EXIT_SENTINEL & ~1):
            uc.emu_stop()
        elif self.instr > INSTR_LIMIT_TOTAL:
            print("!! total instruction limit hit")
            uc.emu_stop()
            self.halted = True

    def _assist_ldm(self, address):
        """Emulate ldmia rn!, {reglist} where rn is in reglist (no writeback)."""
        uc = self.uc
        from unicorn.arm_const import UC_ARM_REG_CPSR

        word = BAD_LDM[address]
        if (uc.reg_read(UC_ARM_REG_CPSR) >> 5) & 1:
            return  # not ARM mode; let Unicorn handle/decode it
        base_reg = (word >> 16) & 0xF
        reglist = word & 0xFFFF
        addr = uc.reg_read(base_reg)
        for reg in range(16):
            if (reglist >> reg) & 1:
                val = struct.unpack("<I", uc.mem_read(addr, 4))[0]
                uc.reg_write(reg, val)
                addr += 4
        # base in list: loaded value wins, no writeback (ARM7TDMI behavior)
        uc.reg_write(UC_ARM_REG_PC, address + 4)

    def _dump_regs(self):
        uc = self.uc
        names = [
            (UC_ARM_REG_R0, "r0"),
            (UC_ARM_REG_R1, "r1"),
            (UC_ARM_REG_R2, "r2"),
            (UC_ARM_REG_R3, "r3"),
            (UC_ARM_REG_R4, "r4"),
            (UC_ARM_REG_R5, "r5"),
            (UC_ARM_REG_R6, "r6"),
            (UC_ARM_REG_R7, "r7"),
            (UC_ARM_REG_R8, "r8"),
            (UC_ARM_REG_R9, "r9/sb"),
            (UC_ARM_REG_R10, "r10/sl"),
            (UC_ARM_REG_R11, "r11/fp"),
            (UC_ARM_REG_R12, "r12/ip"),
            (UC_ARM_REG_R13, "sp"),
            (UC_ARM_REG_R14, "lr"),
        ]
        parts = [f"{n}={uc.reg_read(r):08X}" for r, n in names]
        print("   " + " ".join(parts))
        try:
            from unicorn.arm_const import UC_ARM_REG_CPSR

            cpsr = uc.reg_read(UC_ARM_REG_CPSR)
            print(f"   cpsr={cpsr:08X} T={(cpsr >> 5) & 1} mode={cpsr & 0x1F:#x}")
        except Exception:
            pass

    def _disas_at(self, addr, count=4):
        try:
            from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

            md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
            start = addr - 0x08000000
            if 0 <= start < len(self.rom):
                for insn in md.disasm(self.rom[start : start + 16], addr):
                    print(f"   {insn.address:08X}: {insn.mnemonic} {insn.op_str}")
                    count -= 1
                    if count <= 0:
                        break
        except Exception:
            pass

    def _on_mem_invalid(self, uc, access, address, size, value, user):
        pc = uc.reg_read(UC_ARM_REG_PC)
        if address == (EXIT_SENTINEL & ~1):
            uc.emu_stop()
            return True
        kind = {1: "READ_U", 2: "WRITE_U", 3: "READ_P", 4: "WRITE_P", 5: "FETCH"}.get(
            access, str(access)
        )
        print(
            f"!! invalid {kind}({access}) @ {address:08X} (size {size}, val {value:08X}) pc={pc:08X}"
        )
        self._dump_regs()
        self._disas_at(pc)
        print("   recent PCs:", " ".join(f"{p:08X}" for p in list(self.recent)[-12:]))
        self.halted = True
        uc.emu_stop()
        return True

    def _on_intr(self, uc, intno, user):
        pc = uc.reg_read(UC_ARM_REG_PC)
        svc = None
        svc_pc = pc
        for cand in range(pc - 4, pc + 6, 2):
            try:
                h = struct.unpack("<H", uc.mem_read(cand, 2))[0]
            except Exception:
                continue
            if (h & 0xFF00) == 0xDF00:
                svc = h & 0xFF
                svc_pc = cand
                break
        if svc is None:
            print(f"!! intr {intno} at pc={pc:08X} (no svc found)")
            uc.emu_stop()
            self.halted = True
            return
        self.svc_log.append((svc_pc, svc))
        self._svc(svc, svc_pc)

    def _svc(self, svc, svc_pc):
        uc = self.uc
        r0 = uc.reg_read(UC_ARM_REG_R0)
        r1 = uc.reg_read(UC_ARM_REG_R1)
        r2 = uc.reg_read(UC_ARM_REG_R2)
        if len(self.svc_log) < 40:
            print(
                f"   svc {svc:#04x} @ {svc_pc:08X} r0={r0:08X} r1={r1:08X} r2={r2:08X}"
            )
        if svc == 6:  # Div
            d = struct.unpack("<i", struct.pack("<I", r1))[0]
            n = struct.unpack("<i", struct.pack("<I", r0))[0]
            if d == 0:
                print(f"!! Div by ZERO at {svc_pc:08X} (r0={r0:08X})")
                self.div_log.append((svc_pc, r0, r1))
                uc.reg_write(UC_ARM_REG_R0, 0)
                uc.reg_write(UC_ARM_REG_R1, 0)
                uc.reg_write(UC_ARM_REG_R3, 0)
            else:
                q = int(n / d)
                m = n - q * d
                uc.reg_write(UC_ARM_REG_R0, q & 0xFFFFFFFF)
                uc.reg_write(UC_ARM_REG_R1, m & 0xFFFFFFFF)
                uc.reg_write(UC_ARM_REG_R3, abs(q) & 0xFFFFFFFF)
                self.div_log.append((svc_pc, r0, r1))
        elif svc in (0x0B, 0x0C):  # CpuSet / CpuFastSet
            count = r2 & 0x1FFFFF
            fill = bool(r2 & (1 << 24))
            is32 = bool(r2 & (1 << 26)) or svc == 0x0C
            unit = 4 if is32 else 2
            src = r0 & ~(unit - 1)
            dst = r1 & ~(unit - 1)
            for i in range(count):
                if is32:
                    if fill:
                        data = uc.mem_read(src, 4)
                    else:
                        data = uc.mem_read(src + i * 4, 4)
                    uc.mem_write(dst + i * 4, bytes(data))
                else:
                    if fill:
                        data = uc.mem_read(src, 2)
                    else:
                        data = uc.mem_read(src + i * 2, 2)
                    uc.mem_write(dst + i * 2, bytes(data))
        elif svc in (0x11, 0x12):  # LZ77UnComp Wram/Vram
            self._lz77(r0, r1)
        elif svc == 0x13:  # HuffUnComp
            print(f"!! HuffUnComp svc at {svc_pc:08X} — not implemented")
            self.halted = True
            uc.emu_stop()
        elif svc == 8:  # Sqrt
            import math

            uc.reg_write(UC_ARM_REG_R0, int(math.isqrt(r0)))
        elif svc in (0, 1, 2, 3, 4, 5, 0x19, 0x0D):
            pass  # SoftReset/RRR/Halt/Stop/IntrWait/VBlank/SoundBias/Checksum
        else:
            print(f"!! unhandled svc {svc:#x} at {svc_pc:08X}")
            self.halted = True
            uc.emu_stop()
        # Unicorn resumes exactly at the PC we set; set svc_pc+2 with the Thumb
        # bit so execution continues after the svc in Thumb mode.
        uc.reg_write(UC_ARM_REG_PC, (svc_pc + 2) | 1)

    def _lz77(self, src, dst):
        uc = self.uc
        header = self.r32(src)
        size = header >> 8
        src += 4
        out = bytearray()
        while len(out) < size:
            flags = self.r8(src)
            src += 1
            for bit in range(8):
                if len(out) >= size:
                    break
                if flags & 0x80:
                    b0 = self.r8(src)
                    b1 = self.r8(src + 1)
                    src += 2
                    n = (b0 >> 4) + 3
                    disp = ((b0 & 0xF) << 8) | b1
                    for _ in range(n):
                        out.append(out[-disp - 1])
                else:
                    out.append(self.r8(src))
                    src += 1
                flags <<= 1
        uc.mem_write(dst, bytes(out[:size]))

    # -- function call runner --------------------------------------------
    def call(self, addr, args=(), budget=BUDGET, name=""):
        uc = self.uc
        regs = [UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3]
        for reg, val in zip(regs, args):
            uc.reg_write(reg, val & 0xFFFFFFFF)
        uc.reg_write(UC_ARM_REG_R13, 0x03007E00)
        uc.reg_write(UC_ARM_REG_R14, EXIT_SENTINEL)
        uc.reg_write(UC_ARM_REG_PC, addr & ~1)
        self.call_instr = 0
        start_instr = self.instr
        self.recent.clear()
        try:
            uc.emu_start(addr | 1, 0, count=budget)
        except Exception as e:
            pc = uc.reg_read(UC_ARM_REG_PC)
            if pc == (EXIT_SENTINEL & ~1):
                pass  # normal return sentinel
            elif not self.halted:
                print(f"!! emu error in {name or hex(addr)}: {e}")
                print(f"   pc={pc:08X}")
                self._dump_regs()
                self._disas_at(pc)
                try:
                    base = pc & ~0xF
                    print(
                        f"   mem[{base:08X}]:",
                        bytes(uc.mem_read(base, 16)).hex(),
                    )
                except Exception:
                    pass
                print(
                    "   recent PCs:",
                    " ".join(f"{p:08X}" for p in list(self.recent)[-14:]),
                )
                self.halted = True
        used = self.instr - start_instr
        tag = name or f"{addr:08X}"
        if used >= budget:
            print(f"!! BUDGET EXCEEDED in {tag} ({used} insns)")
            pcs = list(self.recent)
            print("   recent PCs:", " ".join(f"{p:08X}" for p in pcs))
            self._hot_loop(pcs)
            self.halted = True
            return False
        print(f"   {tag}: {used} insns")
        return True

    def _hot_loop(self, pcs):
        if len(pcs) < 8:
            return
        from collections import Counter

        c = Counter(pcs)
        top = c.most_common(8)
        print("   hot:", " ".join(f"{a:08X}x{n}" for a, n in top))


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--voice", type=int, default=1)
    ap.add_argument("--ticks", type=int, default=40)
    ap.add_argument("--rom", type=Path, default=ROM_PATH)
    ap.add_argument(
        "--mode",
        choices=["consumer", "wrapper"],
        default="consumer",
        help="consumer: bare 0x08056A30 ticks; wrapper: full IRQ wrapper ticks",
    )
    args = ap.parse_args()

    rom = args.rom.read_bytes()
    h = Harness(rom)

    print("== Gax2New ==")
    for i in range(0x40 // 4):
        h.w32(G_GAX_PARAMS + i * 4, 0)
    if not h.call(GAX2_NEW, [G_GAX_PARAMS], name="Gax2New"):
        return 2

    h.w32(G_GAX_PARAMS + 0x00, G_GAX_MIXBUF)
    h.w32(G_GAX_PARAMS + 0x04, 0x1000)
    h.w32(G_GAX_PARAMS + 0x30, 0x0824BE44)
    flags = h.r16(G_GAX_PARAMS + 0x0E) & ~0x40
    h.w16(G_GAX_PARAMS + 0x0E, flags)
    h.w8 = None
    h.uc.mem_write(G_GAX_PARAMS + 0x3C, b"\x00")

    print("== Gax2Init ==")
    if not h.call(GAX2_INIT, [G_GAX_PARAMS], name="Gax2Init"):
        return 2

    ws = h.r32(G_GAX_WS_PTR)
    print(f"workspace = {ws:08X}")
    if ws == 0:
        print("!! workspace null after Gax2Init")
        return 2
    rb = h.r32(ws + 0x24)
    print(
        f"rate block +0x24 = {rb:08X}  ch(byte+1)={h.r8(rb+1)} samples(u16+4)={h.r16(rb+4)}"
    )
    print(f"mix dest +0x38 = {h.r32(ws + 0x38):08X}")
    print(f"+0x2C = {h.r32(ws + 0x2C):08X} (before song apply)")

    print("== GaxSongApply (catalog music 0) ==")
    module = h.r32(G_MUSIC_TABLE)
    print(f"module = {module:08X}")
    h.w32(G_GAX_PARAMS + 0x34, module)
    if not h.call(GAX_SONG_APPLY, [G_GAX_PARAMS], name="GaxSongApply"):
        return 2
    # vanilla PlayBgm re-runs Gax2Init when wramSize <= 0x1000
    if not h.call(GAX2_INIT, [G_GAX_PARAMS], name="Gax2Init#2"):
        return 2
    ws = h.r32(G_GAX_WS_PTR)
    print(f"+0x2C = {h.r32(ws + 0x2C):08X} (after song apply)")
    rb2 = h.r32(ws + 0x2C)
    if rb2:
        print(f"   rateblock2 ch={h.r8(rb2+1)} samples={h.r16(rb2+4)}")
    print(f"   ws+0x4C={h.r32(ws + 0x4C):08X} ws+0x50={h.r32(ws + 0x50):08X}")

    cfg = h.r8(G_RUNTIME_CFG + CFG_OFF_GAX_AUDIO)
    print(f"custom_gax_audio = {cfg}")
    if cfg != 1:
        print("!! custom_gax_audio not set in ROM config")
        return 2

    print("== GaxAttachSpeech (real code) ==")
    if not h.call(GAX_ATTACH, [], name="GaxAttachSpeech"):
        return 2
    print(f"ws+0x14 = {h.r32(ws + 0x14):08X} (want {G_SPEECH_OBJ:08X})")
    print(f"ws+0x2C = {h.r32(ws + 0x2C):08X} (want {h.r32(ws + 0x24):08X})")
    idx = struct.unpack("<i", struct.pack("<I", h.r32(G_SPEECH_OBJ + 0x798)))[0]
    print(f"obj+0x798 index = {idx} (want -1)")
    print(f"obj+0x000 owner = {h.r32(G_SPEECH_OBJ):08X} (want {G_SPEECH_OWNER:08X})")
    print(f"owner+0x14 table = {h.r32(G_SPEECH_OWNER + 0x14):08X} (want {G_VOICE_TABLE:08X})")
    print(f"obj+0x794 mixstub = {h.r32(G_SPEECH_OBJ + 0x794):08X}")

    print(f"== GaxPlayVoice({args.voice}) ==")
    if not h.call(GAX_PLAY_VOICE, [args.voice, 0], name="GaxPlayVoice"):
        return 2
    idx = struct.unpack("<i", struct.pack("<I", h.r32(G_SPEECH_OBJ + 0x798)))[0]
    print(f"obj+0x798 index = {idx}")
    print(f"flags = {h.r16(G_GAX_PARAMS + 0x0E):04X}")

    ent = G_VOICE_TABLE + args.voice * 8
    data = h.r32(ent)
    size_flags = h.r32(ent + 4)
    print(f"voice entry data={data:08X} sizeFlags={size_flags:08X}")

    print(f"== consumer ticks (0x{CONSUMER:08X}) ==")
    obj = h.r32(ws + 0x14)
    div_at_start = len(h.div_log)
    for tick in range(args.ticks):
        idx = struct.unpack("<i", struct.pack("<I", h.r32(obj + 0x798)))[0]
        if idx == -1:
            print(f"tick {tick}: speech finished (index=-1)")
            break
        ok = h.call(CONSUMER, [obj], budget=1_000_000, name=f"tick{tick}")
        if not ok or h.halted:
            print(f"!! consumer fault at tick {tick}")
            print(
                f"   idx={struct.unpack('<i', struct.pack('<I', h.r32(obj + 0x798)))[0]} "
                f"cursor={h.r32(obj + 0x79C)} bit={h.r32(obj + 0x7A0)}"
            )
            rb3 = h.r32(ws + 0x2C)
            print(
                f"   +0x2C={rb3:08X} +0x38={h.r32(ws + 0x38):08X} "
                f"mixstub={h.r32(obj + 0x794):08X}"
            )
            for d in h.div_log[div_at_start:]:
                print(f"   Div @ {d[0]:08X}: {d[1]:#x} / {d[2]:#x}")
            return 3
        if tick < 3 or tick % 10 == 0:
            dest = h.r32(ws + 0x38)
            samples = struct.unpack("<8h", h.uc.mem_read(dest, 16))
            print(
                f"   tick {tick}: cursor={h.r32(obj + 0x79C)} "
                f"bit={h.r32(obj + 0x7A0)} pcm0-7={samples}"
            )

    if args.mode == "wrapper":
        print("== full wrapper ticks (IRQ path) ==")
        h.watch_writes(0x03005E70, 0x03005E90)
        idx_slot = h.r32(ws + 0x20)
        song_state = h.r32(ws + 0x18 + idx_slot * 4)
        print(f"song idx={idx_slot} state={song_state:08X}")
        print(
            f"ws+0x68 outfn={h.r32(ws + 0x68):08X} ws+0x6c mixfn={h.r32(ws + 0x6C):08X} "
            f"ws+0x5C={h.r8(ws + 0x5C):02X} ws+0x4C={h.r32(ws + 0x4C):08X} ws+0x50={h.r32(ws + 0x50):08X}"
        )
        # restart speech for the wrapper run
        h.w32(obj + 0x79C, 0)
        h.w32(obj + 0x7A0, 0)
        h.w32(obj + 0x798, args.voice)
        for tick in range(args.ticks):
            idx = struct.unpack("<i", struct.pack("<I", h.r32(obj + 0x798)))[0]
            if idx == -1 and tick > 0:
                print(f"wrapper tick {tick}: speech finished")
                break
            # real dispatcher @ 0x08055BCE: wrapper(song_state, ws+0x30 + samples*channels)
            samples = h.r16(h.r32(ws + 0x2C) + 4)
            channels = h.r32(ws + 0x48)
            out_buf = h.r32(ws + 0x30) + samples * channels
            ok = h.call(
                MIXER_WRAPPER, [song_state, out_buf], budget=2_000_000, name=f"wtick{tick}"
            )
            if not ok or h.halted:
                print(f"!! wrapper fault at tick {tick}")
                print(
                    f"   idx={struct.unpack('<i', struct.pack('<I', h.r32(obj + 0x798)))[0]} "
                    f"cursor={h.r32(obj + 0x79C)} bit={h.r32(obj + 0x7A0)}"
                )
                print(
                    f"   +0x2C={h.r32(ws + 0x2C):08X} +0x38={h.r32(ws + 0x38):08X} "
                    f"state={song_state:08X}"
                )
                return 4
            if tick < 3 or tick % 20 == 0:
                dest = h.r32(ws + 0x38)
                samples = struct.unpack("<8h", h.uc.mem_read(dest, 16))
                print(
                    f"   wtick {tick}: cursor={h.r32(obj + 0x79C)} pcm0-7={samples}"
                )

    print("div calls:", len(h.div_log))
    for d in h.div_log[:6]:
        print(f"   Div @ {d[0]:08X}: {d[1]:#x} / {d[2]:#x}")
    print("svc kinds:", sorted({s for _, s in h.svc_log}))
    print("EMUL OK" if not h.halted else "EMUL HALTED")
    return 0 if not h.halted else 1


if __name__ == "__main__":
    raise SystemExit(main())

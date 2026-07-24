#!/usr/bin/env python3
"""Apply LynJump.event stubs and runtime-gated ROM patches."""

import pathlib
import re
import struct
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent
ORG_RE = re.compile(r"ORG\s+\$([0-9A-Fa-f]+)")
WORD_RE = re.compile(r"WORD\s+(.+)")
POIN_RE = re.compile(r"POIN\s+(\w+)")
BOOL_RE = re.compile(
    r"\.(skip_flight_battle|always_run|always_max_health|always_max_bombs|"
    r"all_cannon_data|all_bullet_data|all_impact_data|"
    r"all_key_items|all_overworld_items|custom_enemy_exp|custom_dialogue|"
    r"custom_gun_data|enemy_hp_bars)\s*=\s*(TRUE|FALSE|true|false|1|0)",
    re.IGNORECASE,
)

# Talk-script bank pointer table (7 ROM pointers) + script-ID range pairs.
DIALOGUE_BANK_TABLE_OFF = 0x24EA6C
DIALOGUE_BANK_COUNT = 7
DIALOGUE_ID_RANGE_OFF = 0x5BF3C

SHOOTER_CHEAT_FLAGS = (
    "always_max_health",
    "always_max_bombs",
    "all_cannon_data",
    "all_bullet_data",
    "all_impact_data",
    "all_key_items",
    "all_overworld_items",
    "custom_gun_data",  # Phoenix per-frame revive via UpdateShooterFrame
)

FLIGHT_SKIP_OFF = 0x1749C
FLIGHT_SKIP_LEN = 0x5E

# Shooter per-frame wrapper @ 0x14E70 (14 bytes): long-jump veneer when cheats on.
SHOOTER_FRAME_OFF = 0x14E70
SHOOTER_FRAME_LEN = 0x0E
# Thumb: ldr r3, [pc, #0]; bx r3; .word hook|1
SHOOTER_FRAME_VENEER_HEAD = struct.pack("<HH", 0x4B00, 0x4718)

# Overworld B-to-run: `ands r0, r1` after `movs r0, #2` — replace with `movs r0, #2`
# so the "B held" branch always wins (doubles move speed at player+0x48/+0x54).
ALWAYS_RUN_ANDS_SITES = [
    0x1DE7C,  # speed doubling
    0x1ECFE,  # run anim / state
    0x1ED36,
    0x1ED68,
]
ANDS_R0_R1 = bytes((0x08, 0x40))  # 0x4008
MOVS_R0_2 = bytes((0x02, 0x20))  # 0x2002

# AddExperience @ 0xFDC4 — long-jump veneer when exp_multiplier != 1
# or custom_enemy_exp is enabled.
ADD_EXPERIENCE_OFF = 0xFDC4
ADD_EXPERIENCE_VENEER_LEN = 8
EXP_MULT_RE = re.compile(r"\.exp_multiplier\s*=\s*(\d+)")

# --- Suction (29th Impact Data) ------------------------------------------------
IMPACT_COUNT_OFF = 0xF0A98 + 8  # word[2] of {28,20,28,22}
ONIMPACT_MAX_OFF = 0x300F0  # cmp r3, #0x1B
ONIMPACT_JT_PTR_OFF = 0x30108  # → vanilla 0x0803010C
ONIMPACT_JT_VANILLA_OFF = 0x3010C
IMPACT_DESC_PTR_OFF = 0x3A384  # → vanilla 0x0807901C
IMPACT_DESC_VANILLA_OFF = 0x7901C
IMPACT_DESC_STRIDE = 0x5A
IMPACT_OWN_SYNC_END_OFF = 0x25996  # cmp r2, #0x4C (Gun ID 76)
PLAYER_STATE_MACHINE_OFF = 0x25DB4
PLAYER_HIT_UPDATE_OFF = 0x24E24
PLAYER_SHIP_UPDATE_OFF = 0x15F34
PLAYER_DEATH_FX_OFF = 0x1BA4C
DELETE_ACTOR_OFF = 0x6310
DRAW_ACTORS_OFF = 0x6C0C
INIT_ACTOR_PARAMS_OFF = 0x318B4
EXP_GEM_UPDATE_OFF = 0x4ABEC
LEECH_GEM_UPDATE_OFF = 0x4B188
GUN_ICON_FRAME_OFF = 0x39F30
ABSORB_HANDLER = 0x08030494
VENEER_LEN = 8
# ygodm8 LynJump long stub (16 bytes): bx pc; nop; ldr ip,[pc]; bx ip; .word
LONG_VENEER_LEN = 16
LONG_VENEER_HEAD = struct.pack("<III", 0x46C04778, 0xE59FC000, 0xE12FFF1C)
# Icon helper: out-ptr in r3, and lr must stay as the caller's return.
# push {r0-r3}; ldr r0,[pc,#8]; mov r12,r0; pop {r0-r3}; bx r12; pad; .word hook
ICON_VENEER_LEN = 16
ICON_VENEER_HEAD = struct.pack(
    "<HHHHHH",
    0xB40F,  # push {r0-r3}
    0x4B02,  # ldr r3, [pc, #8] → word at +0xC (scratch; args on stack)
    0x469C,  # mov r12, r3
    0xBC0F,  # pop {r0-r3}
    0x4760,  # bx r12
    0x0000,  # pad
)
VANILLA_IMPACT_COUNT = 28
IS_GUN_DATA_OWNED_OFF = 0x39C4C
GET_ARCHIVE_FILE_START_OFF = 0x37B4
GET_ARCHIVE_FILE_SIZE_OFF = 0x37C8
SHOOTER_FT_OFF = 0x6188C4
GUN_ICON_ANM_INDEX = 230  # 0-based shooter file index
IMPACT_DATA_JSON = ROOT / "src_custom" / "data_structures" / "impact_data.json"
CUSTOM_IMPACT_ENTRY_SIZE = 0x60  # matches CustomImpactEntry in data_structures.h

# --- Auto Target (29th Cannon Data) --------------------------------------------
CANNON_COUNT_OFF = 0xF0A98  # word[0] of {28,20,28,22}
ONCANNON_MAX_OFF = 0x2D780  # cmp r0, #0x1B
ONCANNON_JT_PTR_OFF = 0x2D798  # → vanilla 0x0802D79C
ONCANNON_JT_VANILLA_OFF = 0x2D79C
CANNON_DESC_PTR_OFF = 0x3A36C  # → vanilla 0x080781E4
CANNON_DESC_VANILLA_OFF = 0x781E4
CANNON_DESC_STRIDE = 0x50
VANILLA_CANNON_COUNT = 28
CANNON_DATA_JSON = ROOT / "src_custom" / "data_structures" / "cannon_data.json"
CUSTOM_CANNON_ENTRY_SIZE = 0x58  # matches CustomCannonEntry in data_structures.h
CUSTOM_CANNON_DESC_OFF = 5  # index, id, number, icon_from, fire_from


def load_symbols(elf_path: pathlib.Path):
    output = subprocess.check_output(["arm-none-eabi-nm", str(elf_path)], text=True)
    symbols = {}
    for line in output.splitlines():
        parts = line.strip().split()
        if len(parts) != 3:
            continue
        addr, sym_type, name = parts
        value = int(addr, 16)
        if sym_type in {"T", "t"}:
            value |= 1
        symbols[name] = value
    return symbols


def checked_write(rom: bytearray, start: int, data: bytes, owners: dict, owner: str):
    end = start + len(data)
    if start < 0 or end > len(rom):
        raise ValueError(f"{owner} writes outside ROM bounds: 0x{start:X}-0x{end:X}")
    for offset in range(start, end):
        previous = owners.get(offset)
        if previous is not None and previous != owner:
            raise ValueError(
                f"{owner} overlaps byte 0x{offset:X} already written by {previous}"
            )
    rom[start:end] = data
    for offset in range(start, end):
        owners[offset] = owner


def apply_event(event_path: pathlib.Path, rom: bytearray, symbols, owners: dict):
    cursor = None
    for raw_line in event_path.read_text().splitlines():
        line = raw_line.split("//", 1)[0].strip()
        if not line or line in {"PUSH", "POP"}:
            continue

        match = ORG_RE.fullmatch(line)
        if match:
            cursor = int(match.group(1), 16)
            continue

        if line.startswith("ALIGN"):
            if cursor is None:
                raise ValueError(f"ALIGN before ORG in {event_path}")
            align = int(line.split()[1], 0)
            cursor = (cursor + (align - 1)) & ~(align - 1)
            continue

        match = WORD_RE.fullmatch(line)
        if match:
            if cursor is None:
                raise ValueError(f"WORD before ORG in {event_path}")
            for token in match.group(1).split():
                value = int(token[1:], 16) if token.startswith("$") else int(token, 0)
                checked_write(
                    rom, cursor, struct.pack("<I", value), owners, f"{event_path}:{line}"
                )
                cursor += 4
            continue

        match = POIN_RE.fullmatch(line)
        if match:
            if cursor is None:
                raise ValueError(f"POIN before ORG in {event_path}")
            name = match.group(1)
            if name not in symbols:
                raise KeyError(f"symbol {name} not found for {event_path}")
            checked_write(
                rom,
                cursor,
                struct.pack("<I", symbols[name]),
                owners,
                f"{event_path}:POIN {name}",
            )
            cursor += 4
            continue

        raise ValueError(f"unsupported line in {event_path}: {raw_line}")


def load_runtime_flags() -> dict[str, bool]:
    text = (ROOT / "configs" / "runtime.c").read_text()
    flags = {
        "skip_flight_battle": True,
        "always_run": False,
        "always_max_health": False,
        "always_max_bombs": False,
        "all_cannon_data": False,
        "all_bullet_data": False,
        "all_impact_data": False,
        "all_key_items": False,
        "all_overworld_items": False,
        "custom_enemy_exp": False,
        "custom_dialogue": False,
        "custom_gun_data": True,
        "enemy_hp_bars": False,
    }
    for match in BOOL_RE.finditer(text):
        name = match.group(1)
        flags[name] = match.group(2).upper() in {"TRUE", "1"}
    return flags


def load_exp_multiplier() -> int:
    text = (ROOT / "configs" / "runtime.c").read_text()
    match = EXP_MULT_RE.search(text)
    return int(match.group(1)) if match else 1


def apply_flight_skip_gate(rom: bytearray, owners: dict, enabled: bool):
    if enabled:
        print("runtime: skip_flight_battle=TRUE (in-place hook kept)")
        return
    baserom = (ROOT / "baserom.gba").read_bytes()
    checked_write(
        rom,
        FLIGHT_SKIP_OFF,
        baserom[FLIGHT_SKIP_OFF : FLIGHT_SKIP_OFF + FLIGHT_SKIP_LEN],
        owners,
        "runtime:skip_flight_battle=FALSE",
    )
    print("runtime: skip_flight_battle=FALSE (restored vanilla 0x1749C)")


def apply_always_run(rom: bytearray, owners: dict, enabled: bool):
    if not enabled:
        print("runtime: always_run=FALSE")
        return
    for off in ALWAYS_RUN_ANDS_SITES:
        cur = bytes(rom[off : off + 2])
        if cur != ANDS_R0_R1:
            raise ValueError(
                f"always_run: expected ands at 0x{off:X}, found {cur.hex()}"
            )
        checked_write(rom, off, MOVS_R0_2, owners, f"runtime:always_run@{off:X}")
    print(f"runtime: always_run=TRUE (patched {len(ALWAYS_RUN_ANDS_SITES)} B-checks)")


def apply_shooter_cheats(rom: bytearray, owners: dict, symbols: dict, flags: dict):
    baserom = (ROOT / "baserom.gba").read_bytes()
    enabled = any(flags[name] for name in SHOOTER_CHEAT_FLAGS)
    if not enabled:
        checked_write(
            rom,
            SHOOTER_FRAME_OFF,
            baserom[SHOOTER_FRAME_OFF : SHOOTER_FRAME_OFF + SHOOTER_FRAME_LEN],
            owners,
            "runtime:shooter_cheats=FALSE",
        )
        print("runtime: shooter/inventory cheats=FALSE (vanilla 0x14E70)")
        return

    name = "UpdateShooterFrame__Replacement"
    if name not in symbols:
        raise KeyError(f"symbol {name} not found (needed for shooter cheats)")
    hook = symbols[name]
    checked_write(
        rom,
        SHOOTER_FRAME_OFF,
        SHOOTER_FRAME_VENEER_HEAD + struct.pack("<I", hook),
        owners,
        "runtime:shooter_cheats",
    )
    on = [n for n in SHOOTER_CHEAT_FLAGS if flags[n]]
    print(f"runtime: shooter/inventory cheats ({', '.join(on)}) → 0x{hook:08X}")


def apply_custom_dialogue(rom: bytearray, owners: dict, symbols: dict, enabled: bool):
    baserom = (ROOT / "baserom.gba").read_bytes()
    table_len = DIALOGUE_BANK_COUNT * 4
    range_len = DIALOGUE_BANK_COUNT * 8
    if not enabled:
        checked_write(
            rom,
            DIALOGUE_BANK_TABLE_OFF,
            baserom[DIALOGUE_BANK_TABLE_OFF : DIALOGUE_BANK_TABLE_OFF + table_len],
            owners,
            "runtime:custom_dialogue=FALSE:banks",
        )
        checked_write(
            rom,
            DIALOGUE_ID_RANGE_OFF,
            baserom[DIALOGUE_ID_RANGE_OFF : DIALOGUE_ID_RANGE_OFF + range_len],
            owners,
            "runtime:custom_dialogue=FALSE:ranges",
        )
        print("runtime: custom_dialogue=FALSE (vanilla talk banks)")
        return

    bank_ptrs = []
    for i in range(DIALOGUE_BANK_COUNT):
        name = f"gDialogueBank{i}"
        if name not in symbols:
            raise KeyError(f"symbol {name} not found (needed for custom_dialogue)")
        # Data symbols: use address as-is (no Thumb bit).
        bank_ptrs.append(symbols[name] & ~1)

    checked_write(
        rom,
        DIALOGUE_BANK_TABLE_OFF,
        b"".join(struct.pack("<I", p) for p in bank_ptrs),
        owners,
        "runtime:custom_dialogue:banks",
    )

    if "gDialogueIdRanges" in symbols:
        # Ranges live in append ROM; copy the 14 u32 words into the vanilla table.
        # Prefer embedding via already-linked bytes: read from ROM at symbol.
        ranges_addr = symbols["gDialogueIdRanges"] & ~1
        file_off = ranges_addr - 0x08000000
        if file_off < 0 or file_off + range_len > len(rom):
            raise ValueError(f"gDialogueIdRanges out of ROM: 0x{ranges_addr:08X}")
        checked_write(
            rom,
            DIALOGUE_ID_RANGE_OFF,
            bytes(rom[file_off : file_off + range_len]),
            owners,
            "runtime:custom_dialogue:ranges",
        )
    print(
        "runtime: custom_dialogue=TRUE → banks "
        + ", ".join(f"0x{p:08X}" for p in bank_ptrs)
    )


def apply_exp_hooks(
    rom: bytearray, owners: dict, symbols: dict, multiplier: int, custom_enemy_exp: bool
):
    """Veneer AddExperience only for exp_multiplier.

    custom_enemy_exp is catalog / by-id data for tools and future hooks — it must
    not rewrite gem awards by amount (gems already carry the full actor+0x3C pool).
    """
    del custom_enemy_exp  # kept in signature for call-site compatibility
    baserom = (ROOT / "baserom.gba").read_bytes()
    if multiplier == 1:
        checked_write(
            rom,
            ADD_EXPERIENCE_OFF,
            baserom[ADD_EXPERIENCE_OFF : ADD_EXPERIENCE_OFF + ADD_EXPERIENCE_VENEER_LEN],
            owners,
            "runtime:exp_hooks=off",
        )
        print("runtime: AddExperience vanilla (exp_multiplier=1)")
        return

    name = "AddExperience__Replacement"
    if name not in symbols:
        raise KeyError(f"symbol {name} not found (needed for exp hooks)")
    hook = symbols[name]
    checked_write(
        rom,
        ADD_EXPERIENCE_OFF,
        SHOOTER_FRAME_VENEER_HEAD + struct.pack("<I", hook),
        owners,
        "runtime:exp_hooks",
    )
    print(f"runtime: exp_multiplier={multiplier} → 0x{hook:08X}")


def _sym_file_off(symbols: dict, name: str) -> int:
    if name not in symbols:
        raise KeyError(f"symbol {name} not found (needed for Suction)")
    addr = symbols[name] & ~1
    return addr - 0x08000000


def apply_veneer(rom: bytearray, owners: dict, off: int, hook: int, owner: str):
    if off & 3:
        raise ValueError(
            f"{owner}: long veneer at 0x{off:X} is not 4-byte aligned "
            "(Thumb ldr [pc,#0] would miss the hook word)"
        )
    checked_write(
        rom,
        off,
        SHOOTER_FRAME_VENEER_HEAD + struct.pack("<I", hook),
        owners,
        owner,
    )


def apply_icon_veneer(rom: bytearray, owners: dict, off: int, hook: int, owner: str):
    checked_write(
        rom,
        off,
        ICON_VENEER_HEAD + struct.pack("<I", hook),
        owners,
        owner,
    )


def _shooter_file_range(rom: bytes, index: int) -> tuple[int, int]:
    count = struct.unpack_from("<I", rom, SHOOTER_FT_OFF)[0] - 1
    if not 0 <= index < count:
        raise IndexError(f"shooter file {index} out of range")
    start = SHOOTER_FT_OFF + struct.unpack_from(
        "<I", rom, SHOOTER_FT_OFF + 4 + index * 4
    )[0]
    end = SHOOTER_FT_OFF + struct.unpack_from(
        "<I", rom, SHOOTER_FT_OFF + 4 + (index + 1) * 4
    )[0]
    return start, end


def _load_scn_sprite_palette(rom: bytes) -> list[int]:
    scn_start, _ = _shooter_file_range(rom, 192)  # 0-based 193.scn
    off = scn_start + 0x200
    pal: list[int] = []
    for _ in range(256):
        pb = struct.unpack_from("<H", rom, off)[0]
        off += 2
        pal.extend(
            [(pb & 0x1F) * 8, ((pb >> 5) & 0x1F) * 8, ((pb >> 10) & 0x1F) * 8]
        )
    return pal


def _encode_icon_tiles(png_path: pathlib.Path, palette: list[int]) -> bytes:
    try:
        from PIL import Image
    except ImportError as exc:
        raise RuntimeError("Pillow required to encode Suction icon") from exc

    src = Image.open(png_path).convert("RGBA")
    canvas = Image.new("RGBA", (32, 16), (0, 0, 0, 0))
    canvas.paste(src, (4, 0), src)

    def nearest(r: int, g: int, b: int, a: int) -> int:
        if a < 128:
            return 0
        best, best_d = 0, 1 << 30
        for i in range(16):
            pr, pg, pb = palette[i * 3], palette[i * 3 + 1], palette[i * 3 + 2]
            d = (pr - r) ** 2 + (pg - g) ** 2 + (pb - b) ** 2
            if d < best_d:
                best_d = d
                best = i
        return best

    idx = [
        [nearest(*canvas.getpixel((x, y))) for x in range(32)] for y in range(16)
    ]
    out = bytearray()
    for ty in range(2):
        for tx in range(4):
            for row in range(8):
                for cp in range(4):
                    x = tx * 8 + cp * 2
                    y = ty * 8 + row
                    a = idx[y][x] & 0xF
                    b = idx[y][x + 1] & 0xF
                    out.append(a | (b << 4))
    if len(out) != 256:
        raise ValueError(f"encoded icon tiles length {len(out)}, expected 256")
    return bytes(out)


def _load_custom_icon_pngs(json_path: pathlib.Path, key: str) -> list[pathlib.Path]:
    import json

    data = json.loads(json_path.read_text())
    paths: list[pathlib.Path] = []
    for entry in data.get(key) or []:
        rel = entry.get("icon_png")
        if not rel:
            raise ValueError(f"{json_path}: {key[:-1]} missing icon_png")
        path = ROOT / rel
        if not path.is_file():
            raise FileNotFoundError(f"missing custom {key[:-1]} icon: {path}")
        paths.append(path)
    if not paths:
        raise ValueError(f"{json_path}: no {key}")
    return paths


def _load_custom_cannon_fire_from() -> list[int]:
    import json

    data = json.loads(CANNON_DATA_JSON.read_text())
    return [int(entry["fire_from"]) for entry in data.get("cannons") or []]


def build_extended_gun_icon_anm(
    baserom: bytes, icon_pngs: list[pathlib.Path]
) -> bytes:
    """Rebuild shooter ANM #230 with locked/owned frame pairs for each custom icon."""
    start, end = _shooter_file_range(baserom, GUN_ICON_ANM_INDEX)
    data = baserom[start:end]
    flags, maxpieces, maxbytes, total, tilestart, tilesize = struct.unpack_from(
        "<HHHHII", data, 0
    )
    if total != 196:
        raise ValueError(f"unexpected gun icon ANM frame count {total}")

    palette = _load_scn_sprite_palette(baserom)

    frames: list[tuple[int, bytes, bytes]] = []
    for i in range(total):
        fo = struct.unpack_from("<I", data, 16 + i * 12)[0]
        fs = struct.unpack_from("<I", data, 16 + i * 12 + 4)[0]
        unk = struct.unpack_from("<I", data, 16 + i * 12 + 8)[0]
        ph = bytes(data[fo : fo + 32])
        tiles = bytes(data[tilestart + fs : tilestart + fs + 256])
        frames.append((unk, ph, tiles))

    ph151 = frames[151][1]
    for png in icon_pngs:
        tiles_new = _encode_icon_tiles(png, palette)
        frames.append((256, ph151, tiles_new))  # locked
        frames.append((256, ph151, tiles_new))  # owned

    new_total = len(frames)
    ft_size = new_total * 12
    ph_blob = b"".join(ph for _, ph, _ in frames)
    tile_blob = b"".join(tiles for _, _, tiles in frames)
    new_tilestart = 16 + ft_size + len(ph_blob)
    new_tilesize = len(tile_blob)

    out = bytearray(16)
    struct.pack_into(
        "<HHHHII",
        out,
        0,
        flags,
        maxpieces,
        maxbytes,
        new_total,
        new_tilestart,
        new_tilesize,
    )
    fo_cursor = 16 + ft_size
    fs_cursor = 0
    for unk, ph, tiles in frames:
        out += struct.pack("<III", fo_cursor, fs_cursor, unk)
        fo_cursor += len(ph)
        fs_cursor += len(tiles)
    out += ph_blob
    out += tile_blob
    return bytes(out)


def apply_suction(rom: bytearray, owners: dict, symbols: dict, enabled: bool):
    if not enabled:
        # Restore sites we own when disabled. Clear LynJump.event owners first
        # so player hit/SM long stubs can be reverted.
        baserom = (ROOT / "baserom.gba").read_bytes()
        for off, length, tag in (
            (IMPACT_COUNT_OFF, 4, "count"),
            (ONIMPACT_MAX_OFF, 2, "max"),
            (ONIMPACT_JT_PTR_OFF, 4, "jt_ptr"),
            (IMPACT_DESC_PTR_OFF, 4, "desc_ptr"),
            (IMPACT_OWN_SYNC_END_OFF, 2, "own_sync"),
            (CANNON_COUNT_OFF, 4, "cannon_count"),
            (ONCANNON_MAX_OFF, 2, "cannon_max"),
            (ONCANNON_JT_PTR_OFF, 4, "cannon_jt_ptr"),
            (CANNON_DESC_PTR_OFF, 4, "cannon_desc_ptr"),
            (EXP_GEM_UPDATE_OFF, VENEER_LEN, "exp_gem"),
            (LEECH_GEM_UPDATE_OFF, VENEER_LEN, "leech_gem"),
            (GUN_ICON_FRAME_OFF, ICON_VENEER_LEN, "icon"),
            (IS_GUN_DATA_OWNED_OFF, VENEER_LEN, "owned"),
            (GET_ARCHIVE_FILE_START_OFF, VENEER_LEN, "file_start"),
            (GET_ARCHIVE_FILE_SIZE_OFF, VENEER_LEN, "file_size"),
            (PLAYER_STATE_MACHINE_OFF, LONG_VENEER_LEN, "player_sm"),
            (PLAYER_HIT_UPDATE_OFF, LONG_VENEER_LEN, "player_hit"),
            (PLAYER_SHIP_UPDATE_OFF, LONG_VENEER_LEN, "player_ship"),
            (PLAYER_DEATH_FX_OFF, VENEER_LEN, "player_death_fx"),
            (DELETE_ACTOR_OFF, VENEER_LEN, "delete_actor"),
        ):
            for offset in range(off, off + length):
                owners.pop(offset, None)
            checked_write(
                rom,
                off,
                baserom[off : off + length],
                owners,
                f"runtime:suction=FALSE:{tag}",
            )
        print("runtime: custom_gun_data=FALSE")
        return

    baserom = (ROOT / "baserom.gba").read_bytes()

    jt_off = _sym_file_off(symbols, "gImpactJumpTable")
    desc_off = _sym_file_off(symbols, "gImpactDescTable")
    custom_off = _sym_file_off(symbols, "gCustomImpacts")
    custom_count_off = _sym_file_off(symbols, "gCustomImpactCount")
    custom_count = struct.unpack_from("<H", rom, custom_count_off)[0]
    if custom_count < 1:
        raise ValueError("custom_gun_data: gCustomImpactCount < 1 (build impact_data.json)")

    impact_total = VANILLA_IMPACT_COUNT + custom_count

    # Fill relocated jump table (vanilla handlers + absorb for each custom).
    jt = bytearray(
        baserom[ONIMPACT_JT_VANILLA_OFF : ONIMPACT_JT_VANILLA_OFF + VANILLA_IMPACT_COUNT * 4]
    )
    for _ in range(custom_count):
        jt += struct.pack("<I", ABSORB_HANDLER)
    checked_write(rom, jt_off, bytes(jt), owners, "runtime:gun_data:jt_data")

    # Fill relocated desc table from baserom + gCustomImpacts[].desc.
    desc = bytearray(
        baserom[
            IMPACT_DESC_VANILLA_OFF : IMPACT_DESC_VANILLA_OFF
            + VANILLA_IMPACT_COUNT * IMPACT_DESC_STRIDE
        ]
    )
    desc += b"\0" * (custom_count * IMPACT_DESC_STRIDE)
    entry_size = CUSTOM_IMPACT_ENTRY_SIZE
    for i in range(custom_count):
        base = custom_off + i * entry_size
        index = rom[base]
        slot_desc = bytes(rom[base + 4 : base + 4 + IMPACT_DESC_STRIDE])
        if index >= impact_total:
            raise ValueError(f"custom_gun_data: impact index {index} out of range")
        start = index * IMPACT_DESC_STRIDE
        desc[start : start + IMPACT_DESC_STRIDE] = slot_desc
    checked_write(rom, desc_off, bytes(desc), owners, "runtime:gun_data:desc_data")

    # Impact type count 28 → 28+N.
    checked_write(
        rom,
        IMPACT_COUNT_OFF,
        struct.pack("<I", impact_total),
        owners,
        "runtime:gun_data:count",
    )

    # OnImpact: cmp r3, #0x1B → cmp r3, #(impact_total-1)
    max_index = impact_total - 1
    if rom[ONIMPACT_MAX_OFF : ONIMPACT_MAX_OFF + 2] != bytes((0x1B, 0x2B)):
        if rom[ONIMPACT_MAX_OFF : ONIMPACT_MAX_OFF + 2] != bytes((max_index, 0x2B)):
            raise ValueError(
                f"custom_gun_data: unexpected OnImpact max insn "
                f"{rom[ONIMPACT_MAX_OFF:ONIMPACT_MAX_OFF+2].hex()}"
            )
    checked_write(
        rom,
        ONIMPACT_MAX_OFF,
        bytes((max_index, 0x2B)),
        owners,
        "runtime:gun_data:max",
    )

    # Point OnImpact at relocated JT.
    checked_write(
        rom,
        ONIMPACT_JT_PTR_OFF,
        struct.pack("<I", 0x08000000 + jt_off),
        owners,
        "runtime:gun_data:jt_ptr",
    )

    # Point status-screen desc printer at relocated table.
    checked_write(
        rom,
        IMPACT_DESC_PTR_OFF,
        struct.pack("<I", 0x08000000 + desc_off),
        owners,
        "runtime:gun_data:desc_ptr",
    )

    # --- Custom Cannon Data (mirrors the Impact pipeline above) ---------------
    cannon_jt_off = _sym_file_off(symbols, "gCannonJumpTable")
    cannon_desc_off = _sym_file_off(symbols, "gCannonDescTable")
    custom_cannon_off = _sym_file_off(symbols, "gCustomCannons")
    cannon_count_sym_off = _sym_file_off(symbols, "gCustomCannonCount")
    cannon_count = struct.unpack_from("<H", rom, cannon_count_sym_off)[0]
    if cannon_count < 1:
        raise ValueError("custom_gun_data: gCustomCannonCount < 1 (build cannon_data.json)")

    cannon_total = VANILLA_CANNON_COUNT + cannon_count
    fire_from = _load_custom_cannon_fire_from()
    if cannon_count != len(fire_from):
        raise ValueError(
            f"custom_gun_data: gCustomCannonCount={cannon_count} but JSON has "
            f"{len(fire_from)} cannons"
        )

    # Relocated OnCannon jump table: vanilla branch targets + one reused
    # handler per custom cannon (fire_from picks the vanilla firing pattern).
    vanilla_cannon_jt = [
        struct.unpack_from("<I", baserom, ONCANNON_JT_VANILLA_OFF + i * 4)[0]
        for i in range(VANILLA_CANNON_COUNT)
    ]
    cannon_jt = bytearray(
        baserom[
            ONCANNON_JT_VANILLA_OFF : ONCANNON_JT_VANILLA_OFF + VANILLA_CANNON_COUNT * 4
        ]
    )
    for src in fire_from:
        if not 0 <= src < VANILLA_CANNON_COUNT:
            raise ValueError(f"custom_gun_data: cannon fire_from {src} out of range")
        cannon_jt += struct.pack("<I", vanilla_cannon_jt[src])
    checked_write(rom, cannon_jt_off, bytes(cannon_jt), owners, "runtime:gun_data:cannon_jt_data")

    # Relocated cannon desc table from baserom + gCustomCannons[].desc.
    cannon_desc = bytearray(
        baserom[
            CANNON_DESC_VANILLA_OFF : CANNON_DESC_VANILLA_OFF
            + VANILLA_CANNON_COUNT * CANNON_DESC_STRIDE
        ]
    )
    cannon_desc += b"\0" * (cannon_count * CANNON_DESC_STRIDE)
    for i in range(cannon_count):
        base = custom_cannon_off + i * CUSTOM_CANNON_ENTRY_SIZE
        index = rom[base]
        slot_desc = bytes(
            rom[
                base + CUSTOM_CANNON_DESC_OFF : base
                + CUSTOM_CANNON_DESC_OFF
                + CANNON_DESC_STRIDE
            ]
        )
        if index >= cannon_total:
            raise ValueError(f"custom_gun_data: cannon index {index} out of range")
        start = index * CANNON_DESC_STRIDE
        cannon_desc[start : start + CANNON_DESC_STRIDE] = slot_desc
    checked_write(
        rom, cannon_desc_off, bytes(cannon_desc), owners, "runtime:gun_data:cannon_desc_data"
    )

    # Cannon type count 28 → 28+N.
    checked_write(
        rom,
        CANNON_COUNT_OFF,
        struct.pack("<I", cannon_total),
        owners,
        "runtime:gun_data:cannon_count",
    )

    # OnCannon: cmp r0, #0x1B → cmp r0, #(cannon_total-1)
    cannon_max_index = cannon_total - 1
    cur = bytes(rom[ONCANNON_MAX_OFF : ONCANNON_MAX_OFF + 2])
    if cur not in (bytes((0x1B, 0x28)), bytes((cannon_max_index, 0x28))):
        raise ValueError(f"custom_gun_data: unexpected OnCannon max insn {cur.hex()}")
    checked_write(
        rom,
        ONCANNON_MAX_OFF,
        bytes((cannon_max_index, 0x28)),
        owners,
        "runtime:gun_data:cannon_max",
    )

    checked_write(
        rom,
        ONCANNON_JT_PTR_OFF,
        struct.pack("<I", 0x08000000 + cannon_jt_off),
        owners,
        "runtime:gun_data:cannon_jt_ptr",
    )
    checked_write(
        rom,
        CANNON_DESC_PTR_OFF,
        struct.pack("<I", 0x08000000 + cannon_desc_off),
        owners,
        "runtime:gun_data:cannon_desc_ptr",
    )

    # Ownership sync loop end: cmp r2, #0x4C → last custom Gun Data id.
    # Cannon ids stay out of it: that loop maps id-49 onto gImpactOwned bits.
    last_gun_id = max(rom[custom_off + i * entry_size + 1] for i in range(custom_count))
    if last_gun_id < 76:
        last_gun_id = 76
    expect_vanilla = bytes((0x4C, 0x2A))
    expect_patched = bytes((last_gun_id, 0x2A))
    cur = bytes(rom[IMPACT_OWN_SYNC_END_OFF : IMPACT_OWN_SYNC_END_OFF + 2])
    if cur not in (expect_vanilla, expect_patched):
        raise ValueError(f"custom_gun_data: unexpected own-sync cmp {cur.hex()}")
    checked_write(
        rom,
        IMPACT_OWN_SYNC_END_OFF,
        expect_patched,
        owners,
        "runtime:gun_data:own_sync",
    )

    # Extended gun-icon ANM: custom impact frames at 196+, cannons after them.
    icon_pngs = _load_custom_icon_pngs(IMPACT_DATA_JSON, "impacts")
    if custom_count != len(icon_pngs):
        raise ValueError(
            f"custom_gun_data: gCustomImpactCount={custom_count} but JSON has "
            f"{len(icon_pngs)} icon_png entries"
        )
    cannon_icon_pngs = _load_custom_icon_pngs(CANNON_DATA_JSON, "cannons")
    if cannon_count != len(cannon_icon_pngs):
        raise ValueError(
            f"custom_gun_data: gCustomCannonCount={cannon_count} but JSON has "
            f"{len(cannon_icon_pngs)} icon_png entries"
        )
    icon_pngs = icon_pngs + cannon_icon_pngs
    anm_ext = build_extended_gun_icon_anm(baserom, icon_pngs)
    anm_off = _sym_file_off(symbols, "gGunIconAnmExt")
    size_off = _sym_file_off(symbols, "gGunIconAnmExtSize")
    # Buffer sized by compile_data_structures.py: 16 + (196+2N)*300, 32-byte aligned.
    anm_cap = (16 + (196 + len(icon_pngs) * 2) * 300 + 31) & ~31
    if len(anm_ext) > anm_cap:
        raise ValueError(
            f"custom_gun_data: ANM {len(anm_ext)} bytes exceeds expected "
            f"buffer {anm_cap} (update impact_data.json / rebuild)"
        )
    checked_write(rom, anm_off, anm_ext, owners, "runtime:gun_data:anm_ext")
    checked_write(
        rom, size_off, struct.pack("<I", len(anm_ext)), owners, "runtime:gun_data:anm_size"
    )

    # Veneers (PlayerHitUpdate / PlayerStateMachine use LynJump.event).
    for sym, off, tag, icon in (
        ("ExpGemUpdate__Replacement", EXP_GEM_UPDATE_OFF, "exp_gem", False),
        ("LeechGemUpdate__Replacement", LEECH_GEM_UPDATE_OFF, "leech_gem", False),
        ("IsGunDataOwned__Replacement", IS_GUN_DATA_OWNED_OFF, "owned", False),
        ("GetArchiveFileStart__Replacement", GET_ARCHIVE_FILE_START_OFF, "file_start", False),
        ("GetArchiveFileSize__Replacement", GET_ARCHIVE_FILE_SIZE_OFF, "file_size", False),
        ("GetGunDataIconFrame__Replacement", GUN_ICON_FRAME_OFF, "icon", True),
        ("PlayerDeathFx__Replacement", PLAYER_DEATH_FX_OFF, "player_death_fx", False),
    ):
        if sym not in symbols:
            raise KeyError(f"symbol {sym} not found (needed for custom_gun_data)")
        if icon:
            apply_icon_veneer(rom, owners, off, symbols[sym], f"runtime:gun_data:{tag}")
        else:
            apply_veneer(rom, owners, off, symbols[sym], f"runtime:gun_data:{tag}")

    print(
        "runtime: custom_gun_data=TRUE "
        f"(impact JT @ 0x{0x08000000 + jt_off:08X}, "
        f"desc @ 0x{0x08000000 + desc_off:08X}; "
        f"cannon JT @ 0x{0x08000000 + cannon_jt_off:08X}, "
        f"desc @ 0x{0x08000000 + cannon_desc_off:08X}; "
        f"counts {cannon_total} cannon / {impact_total} impact; "
        f"ANM ext {len(anm_ext)} bytes)"
    )


def apply_enemy_hp_bars(rom: bytearray, owners: dict, symbols: dict, enabled: bool):
    """DrawActors: plain veneer. InitActorParams: icon veneer (preserves r3=expPool)."""
    baserom = (ROOT / "baserom.gba").read_bytes()
    if not enabled:
        checked_write(
            rom,
            DRAW_ACTORS_OFF,
            baserom[DRAW_ACTORS_OFF : DRAW_ACTORS_OFF + VENEER_LEN],
            owners,
            "runtime:enemy_hp_bars=FALSE:draw",
        )
        checked_write(
            rom,
            INIT_ACTOR_PARAMS_OFF,
            baserom[INIT_ACTOR_PARAMS_OFF : INIT_ACTOR_PARAMS_OFF + ICON_VENEER_LEN],
            owners,
            "runtime:enemy_hp_bars=FALSE:init",
        )
        print("runtime: enemy_hp_bars=FALSE (vanilla DrawActors / InitActorParams)")
        return

    for sym in ("DrawActors__Replacement", "InitActorParams__Replacement"):
        if sym not in symbols:
            raise KeyError(f"symbol {sym} not found (needed for enemy_hp_bars)")

    apply_veneer(
        rom,
        owners,
        DRAW_ACTORS_OFF,
        symbols["DrawActors__Replacement"],
        "runtime:enemy_hp_bars:draw",
    )
    # Plain ldr r3 / bx r3 would clobber expPool (4th arg) → gems award the hook
    # address as EXP and the HUD digit writer overflows into NEXT:.
    apply_icon_veneer(
        rom,
        owners,
        INIT_ACTOR_PARAMS_OFF,
        symbols["InitActorParams__Replacement"],
        "runtime:enemy_hp_bars:init",
    )
    print(
        "runtime: enemy_hp_bars=TRUE → "
        f"DrawActors 0x{symbols['DrawActors__Replacement']:08X}, "
        f"InitActorParams 0x{symbols['InitActorParams__Replacement']:08X} "
        "(r0-r3 preserved)"
    )


def main():
    if len(sys.argv) != 3:
        print(f"usage: {sys.argv[0]} <elf> <rom>", file=sys.stderr)
        return 1

    elf_path = pathlib.Path(sys.argv[1])
    rom_path = pathlib.Path(sys.argv[2])
    event_path = ROOT / "src_custom" / "LynJump.event"

    rom = bytearray(rom_path.read_bytes())
    symbols = load_symbols(elf_path)
    owners: dict[int, str] = {}
    flags = load_runtime_flags()
    exp_multiplier = load_exp_multiplier()

    if event_path.exists():
        apply_event(event_path, rom, symbols, owners)

    apply_flight_skip_gate(rom, owners, flags["skip_flight_battle"])
    apply_always_run(rom, owners, flags["always_run"])
    apply_shooter_cheats(rom, owners, symbols, flags)
    apply_exp_hooks(
        rom, owners, symbols, exp_multiplier, flags["custom_enemy_exp"]
    )
    apply_custom_dialogue(rom, owners, symbols, flags["custom_dialogue"])
    apply_suction(rom, owners, symbols, flags["custom_gun_data"])
    apply_enemy_hp_bars(rom, owners, symbols, flags["enemy_hp_bars"])

    rom_path.write_bytes(rom)
    print(f"patches: {len(owners)} bytes written")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

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
    r"all_key_items|all_tools|level_cap_255|start_max_level|custom_enemy_exp|"
    r"overworld_enemy_exp|custom_dialogue|"
    r"custom_gun_data|enemy_hp_bars|disable_random_battles|custom_cutscene_ch1|"
    r"custom_cutscene_stage|custom_talk_helpers|custom_event_runner|custom_gax_audio)"
    r"\s*=\s*(TRUE|FALSE|true|false|1|0)",
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
    "all_tools",
    "start_max_level",
    "custom_gun_data",  # Phoenix per-frame revive via UpdateShooterFrame
)

# Overworld unlocks (tools / items / max level) before flight.
OVERWORLD_UNLOCK_FLAGS = (
    "all_key_items",
    "all_tools",
    "start_max_level",
)
OVERWORLD_PLAYER_UPDATE_OFF = 0x1DC84

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
# and/or level_cap_255 (custom_enemy_exp is catalog-only and does not veneer).
ADD_EXPERIENCE_OFF = 0xFDC4
ADD_EXPERIENCE_VENEER_LEN = 8
EXP_MULT_RE = re.compile(r"\.exp_multiplier\s*=\s*(\d+)")
# RebuildExpDigits (@ 0xFE20): cmp level,#0x62 → zero remaining EXP at vanilla max.
# Raise to #0xFE so the bar keeps working through level 254 (cap at 255).
LEVEL_CAP_HUD_CMP_SITES = (0xFEB4, 0xFF3E)
CMP_R0_0x62 = bytes((0x62, 0x28))  # cmp r0, #0x62
CMP_R0_0xFE = bytes((0xFE, 0x28))  # cmp r0, #0xFE
# Status overlay LEVEL digits (fall-through @ 0x542B6).
# Site is 2-mod-4, so ARM long veneer (bx pc) is illegal — use Thumb-only:
#   ldr r3, [pc, #4]; bx r3; nop; .word hook|1
STATUS_LEVEL_DIGITS_OFF = 0x542B6
STATUS_LEVEL_DIGITS_VENEER_LEN = 12
# Thumb: ldr r3,[pc,#4] → Align(PC,4)+4 = 0x542BC when site is 0x542B6
STATUS_LEVEL_DIGITS_VENEER_HEAD = struct.pack("<HHH", 0x4B01, 0x4718, 0x0000)

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
DAMAGE_APPLY_OFF = 0x5350
CUTSCENE_CH1_OPENER_OFF = 0x523EC  # CutsceneCh1Opener peel
CUTSCENE_STAGE_OFF = 0x2B18C  # CutsceneStageUpdate peel
START_TALK_PTR_OFF = 0x10808
START_TALK_BY_ID_OFF = 0x108B0
START_TALK_BY_ID_EX_OFF = 0x1093C
BUILD_TALK_OFFSETS_OFF = 0x10964
INIT_TALK_BANKS_OFF = 0x109A0
PLAYER_DEATH_FX_OFF = 0x1BA4C
DELETE_ACTOR_OFF = 0x6310
DRAW_ACTORS_OFF = 0x6C0C
INIT_ACTOR_PARAMS_OFF = 0x318B4
ACTOR_DEATH_AWARD_OFF = 0x319B0
# True random-encounter starter @ 0x1DA5C (not ScanEncounters / lure circles).
# CodeBreaker "no random battles" writes 0x01 to IWRAM 0x03007685 (byte1 of the
# cooldown word at 0x03007684); non-zero cooldown makes 0x1DA5C early-out.
RANDOM_BATTLE_START_OFF = 0x1DA5C
RANDOM_BATTLE_START_DISABLE = bytes((0x70, 0x47))  # bx lr
# Cleanup: prior mistaken ScanEncounters patches (entry + internal BLs).
SCAN_ENCOUNTERS_OFF = 0x146B4
SCAN_ENCOUNTERS_CLEANUP_SITES = [
    (0x146B4, 8),
    (0x147B2, 6),
    (0x147CA, 6),
]
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

# --- Laser (21st Bullet Data) --------------------------------------------------
BULLET_COUNT_OFF = 0xF0A98 + 4  # word[1] of {28,20,28,22}
ONBULLET_MAX_OFF = 0x2E254  # cmp r0, #0x13
ONBULLET_JT_PTR_OFF = 0x2E268  # → vanilla 0x0802E26C
ONBULLET_JT_VANILLA_OFF = 0x2E26C
BULLET_DESC_PTR_OFF = 0x3A378  # → vanilla 0x08078AA4
BULLET_DESC_VANILLA_OFF = 0x78AA4
BULLET_DESC_STRIDE = 0x46
VANILLA_BULLET_COUNT = 20
BULLET_DATA_JSON = ROOT / "src_custom" / "data_structures" / "bullet_data.json"
CUSTOM_BULLET_ENTRY_SIZE = 0x4C  # matches CustomBulletEntry in data_structures.h
CUSTOM_BULLET_DESC_OFF = 5  # index, id, number, icon_from, shot_from
ABSORB_SHOT_OFF = 0x2F58C  # despawn player shot on hit (Pass Through skips this)


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
        "all_tools": False,
        "level_cap_255": False,
        "start_max_level": False,
        "custom_enemy_exp": False,
        "overworld_enemy_exp": False,
        "custom_dialogue": False,
        "custom_gun_data": True,
        "enemy_hp_bars": False,
        "disable_random_battles": False,
        "custom_cutscene_ch1": False,
        "custom_cutscene_stage": False,
        "custom_talk_helpers": False,
        "custom_event_runner": False,
        "custom_gax_audio": False,
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


def apply_overworld_unlocks(rom: bytearray, owners: dict, symbols: dict, flags: dict):
    """OverworldPlayerUpdate @ 0x1DC84 — apply tools/items before any flight stage."""
    baserom = (ROOT / "baserom.gba").read_bytes()
    enabled = any(flags[name] for name in OVERWORLD_UNLOCK_FLAGS)
    if not enabled:
        checked_write(
            rom,
            OVERWORLD_PLAYER_UPDATE_OFF,
            baserom[
                OVERWORLD_PLAYER_UPDATE_OFF : OVERWORLD_PLAYER_UPDATE_OFF
                + VENEER_LEN
            ],
            owners,
            "runtime:overworld_unlocks=FALSE",
        )
        print("runtime: overworld unlocks=FALSE (vanilla 0x1DC84)")
        return

    name = "OverworldPlayerUpdate__Replacement"
    if name not in symbols:
        raise KeyError(f"symbol {name} not found (needed for overworld unlocks)")
    apply_veneer(
        rom,
        owners,
        OVERWORLD_PLAYER_UPDATE_OFF,
        symbols[name],
        "runtime:overworld_unlocks",
    )
    on = [n for n in OVERWORLD_UNLOCK_FLAGS if flags[n]]
    print(
        f"runtime: overworld unlocks ({', '.join(on)}) → "
        f"0x{symbols[name]:08X}"
    )


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
    rom: bytearray,
    owners: dict,
    symbols: dict,
    multiplier: int,
    custom_enemy_exp: bool,
    level_cap_255: bool,
):
    """Veneer AddExperience for exp_multiplier and/or level_cap_255.

    custom_enemy_exp is catalog / by-id data for tools and future hooks — it must
    not rewrite gem awards by amount (gems already carry the full actor+0x3C pool).
    """
    del custom_enemy_exp  # kept in signature for call-site compatibility
    baserom = (ROOT / "baserom.gba").read_bytes()
    need_veneer = multiplier != 1 or level_cap_255

    if not need_veneer:
        checked_write(
            rom,
            ADD_EXPERIENCE_OFF,
            baserom[ADD_EXPERIENCE_OFF : ADD_EXPERIENCE_OFF + ADD_EXPERIENCE_VENEER_LEN],
            owners,
            "runtime:exp_hooks=off",
        )
        print("runtime: AddExperience vanilla (exp_multiplier=1, level_cap_255=FALSE)")
    else:
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
        print(
            f"runtime: exp_multiplier={multiplier} level_cap_255={level_cap_255} "
            f"→ 0x{hook:08X}"
        )

    # HUD remaining-EXP zeroing at vanilla max (level > 98).
    # With level_cap_255, raise the threshold to level > 254.
    for off in LEVEL_CAP_HUD_CMP_SITES:
        cur = bytes(rom[off : off + 2])
        vanilla = baserom[off : off + 2]
        if vanilla != CMP_R0_0x62:
            raise ValueError(
                f"level_cap_255: baserom cmp at 0x{off:X} is {vanilla.hex()}, "
                f"expected {CMP_R0_0x62.hex()}"
            )
        if cur not in (CMP_R0_0x62, CMP_R0_0xFE):
            raise ValueError(
                f"level_cap_255: unexpected cmp at 0x{off:X}, found {cur.hex()}"
            )
        patch = CMP_R0_0xFE if level_cap_255 else vanilla
        checked_write(
            rom,
            off,
            patch,
            owners,
            f"runtime:level_cap_255:hud@{off:X}",
        )
    print(
        f"runtime: level_cap_255={'TRUE' if level_cap_255 else 'FALSE'} "
        f"(HUD cmp sites → {'#0xFE' if level_cap_255 else 'vanilla #0x62'})"
    )

    # Status overlay (SELECT): 3-digit LEVEL. Do not touch the top flight HUD bar.
    name = "DrawStatusLevelDigits__Replacement"
    if level_cap_255:
        if name not in symbols:
            raise KeyError(f"symbol {name} not found (needed for level_cap_255 HUD)")
        hook = symbols[name] | 1  # Thumb
        checked_write(
            rom,
            STATUS_LEVEL_DIGITS_OFF,
            STATUS_LEVEL_DIGITS_VENEER_HEAD + struct.pack("<I", hook),
            owners,
            "runtime:level_cap_255:status_digits",
        )
        print(f"runtime: level_cap_255 status LEVEL digits → 0x{hook:08X}")
    else:
        checked_write(
            rom,
            STATUS_LEVEL_DIGITS_OFF,
            baserom[
                STATUS_LEVEL_DIGITS_OFF : STATUS_LEVEL_DIGITS_OFF
                + STATUS_LEVEL_DIGITS_VENEER_LEN
            ],
            owners,
            "runtime:level_cap_255:status_digits=off",
        )


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
        # Vanilla overrides (bullet index < 20) may omit icon_png.
        if key == "bullets" and int(entry.get("index", 0)) < VANILLA_BULLET_COUNT:
            continue
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


def _load_bullet_json_entries() -> list[dict]:
    import json

    data = json.loads(BULLET_DATA_JSON.read_text())
    return list(data.get("bullets") or [])


def _load_custom_bullet_shot_from() -> list[int]:
    return [
        int(entry["shot_from"])
        for entry in _load_bullet_json_entries()
        if int(entry["index"]) >= VANILLA_BULLET_COUNT
    ]


def _load_bullet_override_descs() -> list[tuple[int, bytes]]:
    """Vanilla-index reworks from bullet_data.json → (index, padded desc)."""
    out: list[tuple[int, bytes]] = []
    for entry in _load_bullet_json_entries():
        index = int(entry["index"])
        if index >= VANILLA_BULLET_COUNT:
            continue
        name = str(entry["name"]).strip()
        text = str(entry["text"]).strip()
        desc = f"{name} : {text} ".encode("ascii")
        if len(desc) >= BULLET_DESC_STRIDE:
            raise ValueError(f"bullet override {index}: desc too long")
        slot = bytearray(BULLET_DESC_STRIDE)
        slot[: len(desc)] = desc
        out.append((index, bytes(slot)))
    return out


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
            (BULLET_COUNT_OFF, 4, "bullet_count"),
            (ONBULLET_MAX_OFF, 2, "bullet_max"),
            (ONBULLET_JT_PTR_OFF, 4, "bullet_jt_ptr"),
            (BULLET_DESC_PTR_OFF, 4, "bullet_desc_ptr"),
            (ABSORB_SHOT_OFF, VENEER_LEN, "absorb_shot"),
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

    # --- Custom Bullet Data (mirrors the Cannon pipeline above) ----------------
    bullet_jt_off = _sym_file_off(symbols, "gBulletJumpTable")
    bullet_desc_off = _sym_file_off(symbols, "gBulletDescTable")
    custom_bullet_off = _sym_file_off(symbols, "gCustomBullets")
    bullet_count_sym_off = _sym_file_off(symbols, "gCustomBulletCount")
    bullet_count = struct.unpack_from("<H", rom, bullet_count_sym_off)[0]
    if bullet_count < 1:
        raise ValueError("custom_gun_data: gCustomBulletCount < 1 (build bullet_data.json)")

    bullet_total = VANILLA_BULLET_COUNT + bullet_count
    shot_from = _load_custom_bullet_shot_from()
    if bullet_count != len(shot_from):
        raise ValueError(
            f"custom_gun_data: gCustomBulletCount={bullet_count} but JSON has "
            f"{len(shot_from)} bullets"
        )

    vanilla_bullet_jt = [
        struct.unpack_from("<I", baserom, ONBULLET_JT_VANILLA_OFF + i * 4)[0]
        for i in range(VANILLA_BULLET_COUNT)
    ]
    bullet_jt = bytearray(
        baserom[
            ONBULLET_JT_VANILLA_OFF : ONBULLET_JT_VANILLA_OFF + VANILLA_BULLET_COUNT * 4
        ]
    )
    for src in shot_from:
        if not 0 <= src < VANILLA_BULLET_COUNT:
            raise ValueError(f"custom_gun_data: bullet shot_from {src} out of range")
        bullet_jt += struct.pack("<I", vanilla_bullet_jt[src])
    checked_write(
        rom, bullet_jt_off, bytes(bullet_jt), owners, "runtime:gun_data:bullet_jt_data"
    )

    bullet_desc = bytearray(
        baserom[
            BULLET_DESC_VANILLA_OFF : BULLET_DESC_VANILLA_OFF
            + VANILLA_BULLET_COUNT * BULLET_DESC_STRIDE
        ]
    )
    for index, slot_desc in _load_bullet_override_descs():
        start = index * BULLET_DESC_STRIDE
        bullet_desc[start : start + BULLET_DESC_STRIDE] = slot_desc
    bullet_desc += b"\0" * (bullet_count * BULLET_DESC_STRIDE)
    for i in range(bullet_count):
        base = custom_bullet_off + i * CUSTOM_BULLET_ENTRY_SIZE
        index = rom[base]
        slot_desc = bytes(
            rom[
                base + CUSTOM_BULLET_DESC_OFF : base
                + CUSTOM_BULLET_DESC_OFF
                + BULLET_DESC_STRIDE
            ]
        )
        if index >= bullet_total:
            raise ValueError(f"custom_gun_data: bullet index {index} out of range")
        if index < VANILLA_BULLET_COUNT:
            raise ValueError(
                f"custom_gun_data: gCustomBullets[{i}] index {index} is vanilla; "
                "put reworks in bullet_data.json as overrides only"
            )
        start = index * BULLET_DESC_STRIDE
        bullet_desc[start : start + BULLET_DESC_STRIDE] = slot_desc
    checked_write(
        rom, bullet_desc_off, bytes(bullet_desc), owners, "runtime:gun_data:bullet_desc_data"
    )

    checked_write(
        rom,
        BULLET_COUNT_OFF,
        struct.pack("<I", bullet_total),
        owners,
        "runtime:gun_data:bullet_count",
    )

    # OnBullet: cmp r0, #0x13 → cmp r0, #(bullet_total-1)
    bullet_max_index = bullet_total - 1
    cur = bytes(rom[ONBULLET_MAX_OFF : ONBULLET_MAX_OFF + 2])
    if cur not in (bytes((0x13, 0x28)), bytes((bullet_max_index, 0x28))):
        raise ValueError(f"custom_gun_data: unexpected OnBullet max insn {cur.hex()}")
    checked_write(
        rom,
        ONBULLET_MAX_OFF,
        bytes((bullet_max_index, 0x28)),
        owners,
        "runtime:gun_data:bullet_max",
    )

    checked_write(
        rom,
        ONBULLET_JT_PTR_OFF,
        struct.pack("<I", 0x08000000 + bullet_jt_off),
        owners,
        "runtime:gun_data:bullet_jt_ptr",
    )
    checked_write(
        rom,
        BULLET_DESC_PTR_OFF,
        struct.pack("<I", 0x08000000 + bullet_desc_off),
        owners,
        "runtime:gun_data:bullet_desc_ptr",
    )

    # Ownership sync loop end: cmp r2, #0x4C → last custom Gun Data id.
    # Cannon / bullet ids stay out of it: that loop maps id-49 onto gImpactOwned bits.
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

    # Extended gun-icon ANM: impacts @196+, cannons, then bullets.
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
    bullet_icon_pngs = _load_custom_icon_pngs(BULLET_DATA_JSON, "bullets")
    if bullet_count != len(bullet_icon_pngs):
        raise ValueError(
            f"custom_gun_data: gCustomBulletCount={bullet_count} but JSON has "
            f"{len(bullet_icon_pngs)} icon_png entries"
        )
    icon_pngs = icon_pngs + cannon_icon_pngs + bullet_icon_pngs
    anm_ext = build_extended_gun_icon_anm(baserom, icon_pngs)
    anm_off = _sym_file_off(symbols, "gGunIconAnmExt")
    size_off = _sym_file_off(symbols, "gGunIconAnmExtSize")
    # Buffer sized by compile_data_structures.py: 16 + (196+2N)*300, 32-byte aligned.
    anm_cap = (16 + (196 + len(icon_pngs) * 2) * 300 + 31) & ~31
    if len(anm_ext) > anm_cap:
        raise ValueError(
            f"custom_gun_data: ANM {len(anm_ext)} bytes exceeds expected "
            f"buffer {anm_cap} (update bullet_data.json / rebuild)"
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
        ("AbsorbShot__Replacement", ABSORB_SHOT_OFF, "absorb_shot", False),
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
        f"bullet JT @ 0x{0x08000000 + bullet_jt_off:08X}, "
        f"desc @ 0x{0x08000000 + bullet_desc_off:08X}; "
        f"counts {cannon_total} cannon / {bullet_total} bullet / {impact_total} impact; "
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


def apply_overworld_enemy_exp(
    rom: bytearray, owners: dict, symbols: dict, enabled: bool, gun_data: bool
):
    """Award EXP on overworld fauna kills.

    Primary: DamageApply @ 0x5350 when HP drops to <=0 (HitScan/tool hits).
    Fallback: PlayerDeathFx @ 0x1BA4C (fauna AI death-drop), piggybacked when
    .custom_gun_data already veneered it for Phoenix.

    Never veneer DeleteActor (too hot / Phoenix softlock history).
    """
    baserom = (ROOT / "baserom.gba").read_bytes()

    def restore(off: int, length: int, tag: str) -> None:
        for offset in range(off, off + length):
            owners.pop(offset, None)
        checked_write(
            rom,
            off,
            baserom[off : off + length],
            owners,
            tag,
        )

    # Undo any prior hot DeleteActor / ActorDeathAward hooks from older builds.
    restore(DELETE_ACTOR_OFF, VENEER_LEN, "runtime:overworld_enemy_exp:restore_delete")
    restore(
        ACTOR_DEATH_AWARD_OFF,
        VENEER_LEN,
        "runtime:overworld_enemy_exp:restore_death_award",
    )

    if not enabled:
        restore(DAMAGE_APPLY_OFF, VENEER_LEN, "runtime:overworld_enemy_exp:restore_dmg")
        print("runtime: overworld_enemy_exp=FALSE")
        return

    dmg_name = "DamageApply__Replacement"
    if dmg_name not in symbols:
        raise KeyError(f"symbol {dmg_name} not found (needed for overworld_enemy_exp)")
    for offset in range(DAMAGE_APPLY_OFF, DAMAGE_APPLY_OFF + VENEER_LEN):
        owners.pop(offset, None)
    apply_veneer(
        rom,
        owners,
        DAMAGE_APPLY_OFF,
        symbols[dmg_name],
        "runtime:overworld_enemy_exp:damage_apply",
    )
    print(
        f"runtime: overworld_enemy_exp=TRUE → DamageApply 0x{symbols[dmg_name]:08X}"
    )

    death_name = "PlayerDeathFx__Replacement"
    if death_name not in symbols:
        raise KeyError(f"symbol {death_name} not found (needed for overworld_enemy_exp)")

    hook = symbols[death_name] | 1
    expected = SHOOTER_FRAME_VENEER_HEAD + struct.pack("<I", hook)
    cur = bytes(rom[PLAYER_DEATH_FX_OFF : PLAYER_DEATH_FX_OFF + VENEER_LEN])
    if cur == expected:
        print(
            "runtime: overworld_enemy_exp=TRUE → piggyback PlayerDeathFx "
            f"0x{symbols[death_name]:08X}"
        )
    else:
        for offset in range(PLAYER_DEATH_FX_OFF, PLAYER_DEATH_FX_OFF + VENEER_LEN):
            owners.pop(offset, None)
        apply_veneer(
            rom,
            owners,
            PLAYER_DEATH_FX_OFF,
            symbols[death_name],
            "runtime:overworld_enemy_exp:death_fx",
        )
        print(
            "runtime: overworld_enemy_exp=TRUE → PlayerDeathFx "
            f"0x{symbols[death_name]:08X}"
        )
    del gun_data  # reserved for call-site symmetry / future gating


def apply_cutscene_ch1(rom: bytearray, owners: dict, symbols: dict, enabled: bool,
                       event_runner: bool = False):
    """LynJump Chapter 1 opener FSM @ 0x523EC.

    Prefer CutsceneCh1Opener__Replacement when custom_event_runner is on;
    otherwise CutsceneCh1Opener peel.
    """
    baserom = (ROOT / "baserom.gba").read_bytes()
    owner = "runtime:custom_cutscene_ch1"

    if not enabled and not event_runner:
        for offset in range(CUTSCENE_CH1_OPENER_OFF, CUTSCENE_CH1_OPENER_OFF + VENEER_LEN):
            owners.pop(offset, None)
        checked_write(
            rom,
            CUTSCENE_CH1_OPENER_OFF,
            baserom[CUTSCENE_CH1_OPENER_OFF : CUTSCENE_CH1_OPENER_OFF + VENEER_LEN],
            owners,
            f"{owner}=FALSE",
        )
        print("runtime: custom_cutscene_ch1=FALSE (vanilla Ch.1 opener)")
        return

    if event_runner and "CutsceneCh1Opener__Replacement" in symbols:
        name = "CutsceneCh1Opener__Replacement"
    else:
        name = "CutsceneCh1Opener"
    if name not in symbols:
        raise KeyError(f"symbol {name} not found (needed for custom_cutscene_ch1)")
    for offset in range(CUTSCENE_CH1_OPENER_OFF, CUTSCENE_CH1_OPENER_OFF + VENEER_LEN):
        owners.pop(offset, None)
    apply_veneer(
        rom,
        owners,
        CUTSCENE_CH1_OPENER_OFF,
        symbols[name],
        owner,
    )
    print(
        f"runtime: custom_cutscene_ch1/event_runner → {name} "
        f"0x{symbols[name]:08X}"
    )


def apply_cutscene_stage(rom: bytearray, owners: dict, symbols: dict, enabled: bool):
    """LynJump stage FSM @ 0x2B18C → CutsceneStageUpdate peel."""
    baserom = (ROOT / "baserom.gba").read_bytes()
    owner = "runtime:custom_cutscene_stage"

    if not enabled:
        for offset in range(CUTSCENE_STAGE_OFF, CUTSCENE_STAGE_OFF + VENEER_LEN):
            owners.pop(offset, None)
        checked_write(
            rom,
            CUTSCENE_STAGE_OFF,
            baserom[CUTSCENE_STAGE_OFF : CUTSCENE_STAGE_OFF + VENEER_LEN],
            owners,
            f"{owner}=FALSE",
        )
        print("runtime: custom_cutscene_stage=FALSE (vanilla stage FSM)")
        return

    name = "CutsceneStageUpdate"
    if name not in symbols:
        raise KeyError(f"symbol {name} not found (needed for custom_cutscene_stage)")
    for offset in range(CUTSCENE_STAGE_OFF, CUTSCENE_STAGE_OFF + VENEER_LEN):
        owners.pop(offset, None)
    apply_veneer(rom, owners, CUTSCENE_STAGE_OFF, symbols[name], owner)
    print(
        f"runtime: custom_cutscene_stage=TRUE → CutsceneStageUpdate "
        f"0x{symbols[name]:08X}"
    )


def apply_talk_helpers(rom: bytearray, owners: dict, symbols: dict, enabled: bool):
    """LynJump StartTalk* / InitTalkBanks / BuildTalkOffsets peels."""
    baserom = (ROOT / "baserom.gba").read_bytes()
    owner = "runtime:custom_talk_helpers"
    sites = (
        (START_TALK_PTR_OFF, "StartTalkPtr"),
        (START_TALK_BY_ID_OFF, "StartTalkById"),
        (START_TALK_BY_ID_EX_OFF, "StartTalkByIdEx"),
        (BUILD_TALK_OFFSETS_OFF, "BuildTalkOffsets"),
        (INIT_TALK_BANKS_OFF, "InitTalkBanks"),
    )

    if not enabled:
        for off, _name in sites:
            for offset in range(off, off + VENEER_LEN):
                owners.pop(offset, None)
            checked_write(
                rom,
                off,
                baserom[off : off + VENEER_LEN],
                owners,
                f"{owner}=FALSE",
            )
        print("runtime: custom_talk_helpers=FALSE (vanilla talk helpers)")
        return

    for off, name in sites:
        if name not in symbols:
            raise KeyError(f"symbol {name} not found (needed for custom_talk_helpers)")
        for offset in range(off, off + VENEER_LEN):
            owners.pop(offset, None)
        apply_veneer(rom, owners, off, symbols[name], owner)
        print(f"runtime: custom_talk_helpers=TRUE → {name} 0x{symbols[name]:08X}")


def apply_disable_random_battles(rom: bytearray, owners: dict, symbols: dict, enabled: bool):
    """No-op TryStartRandomBattle @ 0x1DA5C when enabled.

    Matches CodeBreaker cheat B581C8AEE38E → write 0x01 @ 0x03007685 (cooldown).
    Lure / ScanEncounters / EnterLureZone stay vanilla.
    """
    del symbols  # kept in signature for call-site uniformity
    baserom = (ROOT / "baserom.gba").read_bytes()
    owner = "runtime:disable_random_battles"

    # Always undo any older ScanEncounters patches from earlier attempts.
    for off, length in SCAN_ENCOUNTERS_CLEANUP_SITES:
        checked_write(
            rom,
            off,
            baserom[off : off + length],
            owners,
            f"{owner}:scan_cleanup@{off:X}",
        )

    if not enabled:
        checked_write(
            rom,
            RANDOM_BATTLE_START_OFF,
            baserom[
                RANDOM_BATTLE_START_OFF : RANDOM_BATTLE_START_OFF
                + len(RANDOM_BATTLE_START_DISABLE)
            ],
            owners,
            f"{owner}=FALSE",
        )
        print("runtime: disable_random_battles=FALSE (vanilla TryStartRandomBattle)")
        return

    checked_write(
        rom,
        RANDOM_BATTLE_START_OFF,
        RANDOM_BATTLE_START_DISABLE,
        owners,
        owner,
    )
    print(
        "runtime: disable_random_battles=TRUE → "
        "TryStartRandomBattle 0x1DA5C bx lr (CB cooldown gate)"
    )


GAX_BOOT_OFF = 0x38D8  # AgbMain: gax2_new … gax2_init
GAX_BOOT_VENEER_LEN = 8
GAX_SPEECH_CONSUMER_OFF = 0x56A30  # temporary probe; restore when flag off
TALK_ADVANCE_OFF = 0x102BC


def apply_gax_audio(rom: bytearray, owners: dict, symbols: dict, enabled: bool):
    """LynJump AgbMain GAX boot + TalkAdvance + temporary speech consumer probe."""
    baserom = (ROOT / "baserom.gba").read_bytes()
    owner = "runtime:custom_gax_audio"
    if not enabled:
        checked_write(
            rom,
            GAX_BOOT_OFF,
            baserom[GAX_BOOT_OFF : GAX_BOOT_OFF + GAX_BOOT_VENEER_LEN],
            owners,
            f"{owner}=FALSE",
        )
        checked_write(
            rom,
            TALK_ADVANCE_OFF,
            baserom[TALK_ADVANCE_OFF : TALK_ADVANCE_OFF + VENEER_LEN],
            owners,
            f"{owner}=FALSE:TalkAdvance",
        )
        checked_write(
            rom,
            GAX_SPEECH_CONSUMER_OFF,
            baserom[GAX_SPEECH_CONSUMER_OFF : GAX_SPEECH_CONSUMER_OFF + VENEER_LEN],
            owners,
            f"{owner}=FALSE:SpeechConsumer",
        )
        print("runtime: custom_gax_audio=FALSE (vanilla GAX boot / TalkAdvance)")
        return

    boot = "GaxBootInit__Replacement"
    if boot not in symbols:
        raise KeyError(f"{boot} missing — build gax_audio_hooks.c")
    apply_veneer(rom, owners, GAX_BOOT_OFF, symbols[boot], owner)
    print(f"runtime: custom_gax_audio=TRUE → {boot} 0x{symbols[boot]:08X}")

    talk = "TalkAdvance_Gax__Replacement"
    if talk not in symbols:
        raise KeyError(f"{talk} missing — build gax_audio_hooks.c")
    for offset in range(TALK_ADVANCE_OFF, TALK_ADVANCE_OFF + VENEER_LEN):
        owners.pop(offset, None)
    apply_veneer(rom, owners, TALK_ADVANCE_OFF, symbols[talk], owner)
    print(f"runtime: custom_gax_audio=TRUE → {talk} 0x{symbols[talk]:08X}")

    consumer = "GaxSpeechConsumer__Replacement"
    if consumer not in symbols:
        raise KeyError(f"{consumer} missing — build gax_audio_hooks.c")
    for offset in range(GAX_SPEECH_CONSUMER_OFF, GAX_SPEECH_CONSUMER_OFF + VENEER_LEN):
        owners.pop(offset, None)
    apply_veneer(rom, owners, GAX_SPEECH_CONSUMER_OFF, symbols[consumer], owner)
    print(f"runtime: custom_gax_audio=TRUE → {consumer} 0x{symbols[consumer]:08X}")


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
    apply_overworld_unlocks(rom, owners, symbols, flags)
    apply_exp_hooks(
        rom,
        owners,
        symbols,
        exp_multiplier,
        flags["custom_enemy_exp"],
        flags["level_cap_255"],
    )
    apply_custom_dialogue(rom, owners, symbols, flags["custom_dialogue"])
    apply_suction(rom, owners, symbols, flags["custom_gun_data"])
    apply_enemy_hp_bars(rom, owners, symbols, flags["enemy_hp_bars"])
    apply_overworld_enemy_exp(
        rom,
        owners,
        symbols,
        flags["overworld_enemy_exp"],
        flags["custom_gun_data"],
    )
    apply_disable_random_battles(rom, owners, symbols, flags["disable_random_battles"])
    apply_cutscene_ch1(
        rom,
        owners,
        symbols,
        flags["custom_cutscene_ch1"],
        flags["custom_event_runner"],
    )
    apply_cutscene_stage(rom, owners, symbols, flags["custom_cutscene_stage"])
    apply_talk_helpers(rom, owners, symbols, flags["custom_talk_helpers"])
    apply_gax_audio(rom, owners, symbols, flags["custom_gax_audio"])

    rom_path.write_bytes(rom)
    print(f"patches: {len(owners)} bytes written")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

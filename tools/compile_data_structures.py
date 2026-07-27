#!/usr/bin/env python3
"""Compile src_custom/data_structures/*.json into APPEND_RODATA C tables.

enemy_exp.json (schema 2): per-enemy catalog → gEnemyExpRemap + gEnemyExpById.
overworld_enemy_exp.json (schema 1): overworld fauna → gOverworldEnemyExp.
impact_data.json (schema 1): custom Impact Data → gCustomImpacts + JT/desc/ANM bufs.
cannon_data.json (schema 1): custom Cannon Data → gCustomCannons + JT/desc bufs.
bullet_data.json (schema 1): Bullet Data overrides (index < 20) + custom
appends → gCustomBullets + JT/desc bufs (overrides applied by apply_lynjump).
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]
IMPACT_DESC_STRIDE = 0x5A
CUSTOM_IMPACT_ENTRY_SIZE = 0x60  # 4 + 0x5A desc + 2 pad
VANILLA_IMPACT_COUNT = 28
CANNON_DESC_STRIDE = 0x50
CUSTOM_CANNON_ENTRY_SIZE = 0x58  # 5 + 0x50 desc + 3 pad
VANILLA_CANNON_COUNT = 28
BULLET_DESC_STRIDE = 0x46
CUSTOM_BULLET_ENTRY_SIZE = 0x4C  # 5 + 0x46 desc + 1 pad
VANILLA_BULLET_COUNT = 20
VANILLA_GUN_ICON_FRAMES = 196
GUN_ICON_FRAME_BYTES = 300  # 12 + 32 + 256 per frame in rebuilt ANM


def load_enemy_exp(path: Path) -> dict:
    data = json.loads(path.read_text())
    schema = data.get("schema")
    if schema not in (1, 2):
        raise ValueError(f"{path}: unsupported schema {schema!r}")
    enemies = data.get("enemies")
    if not isinstance(enemies, list) or not enemies:
        raise ValueError(f"{path}: expected non-empty 'enemies' array")
    return data


def compile_enemy_exp(
    data: dict,
) -> tuple[list[tuple[int, int, str]], list[tuple[int, int, str]]]:
    """Return (remap_rows, by_id_rows).

    exp is the full AddExperience amount (= actor+0x3C pool on the gem).
    Schema 1: optional amount remap vanilla→exp.
    Schema 2: by-id catalog only (no amount remap — that wrongly treated
    pool/10 keys as awards and could not distinguish shared pools).
    """
    schema = data["schema"]
    remap: dict[int, tuple[int, str]] = {}
    by_id: dict[int, tuple[int, str]] = {}

    for i, entry in enumerate(data["enemies"]):
        if not isinstance(entry, dict):
            raise ValueError(f"enemies[{i}]: expected object")

        if schema == 1:
            try:
                key = int(entry["vanilla"])
                exp = int(entry["exp"])
            except (KeyError, TypeError, ValueError) as exc:
                raise ValueError(f"enemies[{i}]: need integer vanilla and exp") from exc
            label = str(entry.get("label", f"{key}_exp"))
            eid = None
            if not 0 <= key <= 0xFFFF or not 0 <= exp <= 0xFFFF:
                raise ValueError(f"enemies[{i}]: values out of u16 range")
            if key not in remap:
                remap[key] = (exp, label)
        else:
            try:
                exp = int(entry["exp"])
                eid = int(entry["id"])
            except (KeyError, TypeError, ValueError) as exc:
                raise ValueError(f"enemies[{i}]: need integer id and exp") from exc
            label = str(entry.get("name") or f"type_{eid}")
            if not 0 <= eid <= 0xFFFF or not 0 <= exp <= 0xFFFF:
                raise ValueError(f"enemies[{i}]: values out of u16 range")
            if eid not in by_id:
                by_id[eid] = (exp, label)

    remap_rows = sorted((k, e, lab) for k, (e, lab) in remap.items())
    by_id_rows = sorted((i, e, lab) for i, (e, lab) in by_id.items())
    return remap_rows, by_id_rows


OW_ENEMY_IDS_H = REPO / "include" / "overworld_enemy_ids.h"
OW_ENUM_RE = re.compile(
    r"\b(OVERWORLD_(?:TYPE|ANIM)_([A-Z0-9_]+))\s*=\s*(\d+)\b"
)


def load_overworld_enemy_id_enums(path: Path = OW_ENEMY_IDS_H) -> dict[str, dict[str, tuple[str, int]]]:
    """Parse OVERWORLD_TYPE_* / OVERWORLD_ANIM_* from overworld_enemy_ids.h."""
    text = path.read_text()
    tables: dict[str, dict[str, tuple[str, int]]] = {"TYPE": {}, "ANIM": {}}
    for full, short, value in OW_ENUM_RE.findall(text):
        kind = "TYPE" if full.startswith("OVERWORLD_TYPE_") else "ANIM"
        tables[kind][short] = (full, int(value))
        tables[kind][full] = (full, int(value))
    if not tables["TYPE"] or not tables["ANIM"]:
        raise ValueError(
            f"{path}: no OVERWORLD_TYPE_* / OVERWORLD_ANIM_* enumerators found"
        )
    return tables


def resolve_overworld_id(raw, kind: str, enums: dict, where: str) -> tuple[str, int]:
    """Resolve JSON id/anim: integer, or name from overworld_enemy_ids.h."""
    table = enums[kind]
    if isinstance(raw, bool):
        raise ValueError(f"{where}: invalid {kind.lower()} {raw!r}")
    if isinstance(raw, int):
        for full, value in table.values():
            if value == raw:
                return full, value
        return str(raw), raw
    if isinstance(raw, str):
        key = raw.strip()
        if key in table:
            return table[key]
        raise ValueError(
            f"{where}: unknown {kind.lower()} {raw!r} "
            f"(add OVERWORLD_{kind}_{key} to include/overworld_enemy_ids.h)"
        )
    raise ValueError(f"{where}: {kind.lower()} must be int or name string")


def load_overworld_enemy_exp(path: Path) -> list[tuple[str, str, int, int, int, str]]:
    """Return rows: (type_sym, anim_sym, type_val, anim_val, exp, label)."""
    enums = load_overworld_enemy_id_enums()
    data = json.loads(path.read_text())
    if data.get("schema") != 1:
        raise ValueError(f"{path}: unsupported schema {data.get('schema')!r}")
    enemies = data.get("enemies")
    if not isinstance(enemies, list) or not enemies:
        raise ValueError(f"{path}: expected non-empty 'enemies' array")

    rows: list[tuple[str, str, int, int, int, str]] = []
    seen: set[tuple[int, int]] = set()
    for i, entry in enumerate(enemies):
        if not isinstance(entry, dict):
            raise ValueError(f"enemies[{i}]: expected object")
        where = f"enemies[{i}]"
        try:
            type_sym, eid = resolve_overworld_id(entry["id"], "TYPE", enums, where)
            anim_sym, anim = resolve_overworld_id(entry["anim"], "ANIM", enums, where)
            exp = int(entry["exp"])
        except KeyError as exc:
            raise ValueError(f"{where}: need id, anim, and exp") from exc
        except (TypeError, ValueError) as exc:
            raise ValueError(f"{where}: {exc}") from exc
        if not 0 <= eid <= 0xFFFF or not 0 <= anim <= 0xFFFF or not 0 <= exp <= 0xFFFF:
            raise ValueError(f"{where}: values out of u16 range")
        key = (eid, anim)
        if key in seen:
            raise ValueError(f"{where}: duplicate id={eid} anim={anim}")
        seen.add(key)
        label = str(entry.get("name") or type_sym)
        rows.append((type_sym, anim_sym, eid, anim, exp, label))
    rows.sort(key=lambda r: (r[2], r[3]))
    return rows



def load_custom_impacts(path: Path) -> list[dict]:
    data = json.loads(path.read_text())
    if data.get("schema") != 1:
        raise ValueError(f"{path}: unsupported schema {data.get('schema')!r}")
    impacts = data.get("impacts")
    if not isinstance(impacts, list) or not impacts:
        raise ValueError(f"{path}: expected non-empty 'impacts' array")

    rows: list[dict] = []
    for i, entry in enumerate(impacts):
        if not isinstance(entry, dict):
            raise ValueError(f"impacts[{i}]: expected object")
        try:
            index = int(entry["index"])
            gun_id = int(entry["id"])
            number = int(entry["number"])
            icon_from = int(entry["icon_from"])
            name = str(entry["name"]).strip()
            text = str(entry["text"]).strip()
        except (KeyError, TypeError, ValueError) as exc:
            raise ValueError(
                f"impacts[{i}]: need index, id, number, icon_from, name, text"
            ) from exc

        if not 0 <= index <= 31 or not 0 <= gun_id <= 255:
            raise ValueError(f"impacts[{i}]: index/id out of range")
        if index < VANILLA_IMPACT_COUNT:
            raise ValueError(
                f"impacts[{i}]: index {index} collides with vanilla "
                f"0..{VANILLA_IMPACT_COUNT - 1}"
            )
        if not 0 <= icon_from <= 27:
            raise ValueError(f"impacts[{i}]: icon_from must be a vanilla impact 0..27")
        if not 1 <= number <= 99:
            raise ValueError(f"impacts[{i}]: number out of range")
        if not entry.get("icon_png"):
            raise ValueError(f"impacts[{i}]: need icon_png")

        desc = f"{name} : {text} "
        if len(desc) >= IMPACT_DESC_STRIDE:
            raise ValueError(
                f"impacts[{i}]: name+text too long for 0x{IMPACT_DESC_STRIDE:X}-byte slot"
            )
        rows.append(
            {
                "index": index,
                "id": gun_id,
                "number": number,
                "icon_from": icon_from,
                "desc": desc,
                "label": name,
                "icon_png": str(entry["icon_png"]),
            }
        )
    return rows


def load_custom_cannons(path: Path) -> list[dict]:
    data = json.loads(path.read_text())
    if data.get("schema") != 1:
        raise ValueError(f"{path}: unsupported schema {data.get('schema')!r}")
    cannons = data.get("cannons")
    if not isinstance(cannons, list) or not cannons:
        raise ValueError(f"{path}: expected non-empty 'cannons' array")

    rows: list[dict] = []
    for i, entry in enumerate(cannons):
        if not isinstance(entry, dict):
            raise ValueError(f"cannons[{i}]: expected object")
        try:
            index = int(entry["index"])
            gun_id = int(entry["id"])
            number = int(entry["number"])
            icon_from = int(entry["icon_from"])
            fire_from = int(entry["fire_from"])
            name = str(entry["name"]).strip()
            text = str(entry["text"]).strip()
        except (KeyError, TypeError, ValueError) as exc:
            raise ValueError(
                f"cannons[{i}]: need index, id, number, icon_from, fire_from, "
                "name, text"
            ) from exc

        if not 0 <= index <= 31 or not 0 <= gun_id <= 255:
            raise ValueError(f"cannons[{i}]: index/id out of range")
        if index < VANILLA_CANNON_COUNT:
            raise ValueError(
                f"cannons[{i}]: index {index} collides with vanilla "
                f"0..{VANILLA_CANNON_COUNT - 1}"
            )
        if not 0 <= icon_from <= 27:
            raise ValueError(f"cannons[{i}]: icon_from must be a vanilla cannon 0..27")
        if not 0 <= fire_from <= 27:
            raise ValueError(f"cannons[{i}]: fire_from must be a vanilla cannon 0..27")
        if not 1 <= number <= 99:
            raise ValueError(f"cannons[{i}]: number out of range")
        if not entry.get("icon_png"):
            raise ValueError(f"cannons[{i}]: need icon_png")

        desc = f"{name} : {text} "
        if len(desc) >= CANNON_DESC_STRIDE:
            raise ValueError(
                f"cannons[{i}]: name+text too long for 0x{CANNON_DESC_STRIDE:X}-byte slot"
            )
        rows.append(
            {
                "index": index,
                "id": gun_id,
                "number": number,
                "icon_from": icon_from,
                "fire_from": fire_from,
                "desc": desc,
                "label": name,
                "icon_png": str(entry["icon_png"]),
            }
        )
    return rows


def load_custom_bullets(path: Path) -> tuple[list[dict], list[dict]]:
    """Return (appends, overrides).

    index >= 20 → new pieces (JT / owned / icon ANM).
    index < 20  → vanilla reworks (desc patched by apply_lynjump only).
    """
    data = json.loads(path.read_text())
    if data.get("schema") != 1:
        raise ValueError(f"{path}: unsupported schema {data.get('schema')!r}")
    bullets = data.get("bullets")
    if not isinstance(bullets, list) or not bullets:
        raise ValueError(f"{path}: expected non-empty 'bullets' array")

    appends: list[dict] = []
    overrides: list[dict] = []
    seen: set[int] = set()
    for i, entry in enumerate(bullets):
        if not isinstance(entry, dict):
            raise ValueError(f"bullets[{i}]: expected object")
        try:
            index = int(entry["index"])
            gun_id = int(entry["id"])
            number = int(entry["number"])
            name = str(entry["name"]).strip()
            text = str(entry["text"]).strip()
        except (KeyError, TypeError, ValueError) as exc:
            raise ValueError(
                f"bullets[{i}]: need index, id, number, name, text"
            ) from exc

        if not 0 <= index <= 31 or not 0 <= gun_id <= 255:
            raise ValueError(f"bullets[{i}]: index/id out of range")
        if index in seen:
            raise ValueError(f"bullets[{i}]: duplicate index {index}")
        seen.add(index)
        if not 1 <= number <= 99:
            raise ValueError(f"bullets[{i}]: number out of range")

        desc = f"{name} : {text} "
        if len(desc) >= BULLET_DESC_STRIDE:
            raise ValueError(
                f"bullets[{i}]: name+text too long for 0x{BULLET_DESC_STRIDE:X}-byte slot"
            )

        is_override = index < VANILLA_BULLET_COUNT
        if is_override:
            icon_from = int(entry["icon_from"]) if "icon_from" in entry else index
            shot_from = int(entry["shot_from"]) if "shot_from" in entry else index
            if not 0 <= icon_from <= 19:
                raise ValueError(
                    f"bullets[{i}]: icon_from must be a vanilla bullet 0..19"
                )
            if not 0 <= shot_from <= 19:
                raise ValueError(
                    f"bullets[{i}]: shot_from must be a vanilla bullet 0..19"
                )
            overrides.append(
                {
                    "index": index,
                    "id": gun_id,
                    "number": number,
                    "icon_from": icon_from,
                    "shot_from": shot_from,
                    "desc": desc,
                    "label": name,
                    "icon_png": str(entry["icon_png"]) if entry.get("icon_png") else "",
                }
            )
            continue

        try:
            icon_from = int(entry["icon_from"])
            shot_from = int(entry["shot_from"])
        except (KeyError, TypeError, ValueError) as exc:
            raise ValueError(
                f"bullets[{i}]: appends need icon_from and shot_from"
            ) from exc
        if not 0 <= icon_from <= 19:
            raise ValueError(f"bullets[{i}]: icon_from must be a vanilla bullet 0..19")
        if not 0 <= shot_from <= 19:
            raise ValueError(f"bullets[{i}]: shot_from must be a vanilla bullet 0..19")
        if not entry.get("icon_png"):
            raise ValueError(f"bullets[{i}]: appends need icon_png")

        appends.append(
            {
                "index": index,
                "id": gun_id,
                "number": number,
                "icon_from": icon_from,
                "shot_from": shot_from,
                "desc": desc,
                "label": name,
                "icon_png": str(entry["icon_png"]),
            }
        )
    if not appends:
        raise ValueError(f"{path}: need at least one appended bullet (index >= 20)")
    return appends, overrides


def c_string_bytes(s: str, size: int) -> str:
    raw = s.encode("ascii") + b"\0"
    if len(raw) > size:
        raise ValueError(f"string too long ({len(raw)} > {size})")
    raw = raw.ljust(size, b"\0")
    return ", ".join(str(b) for b in raw)


def gun_icon_anm_ext_max(custom_count: int) -> int:
    frames = VANILLA_GUN_ICON_FRAMES + custom_count * 2
    size = 16 + frames * GUN_ICON_FRAME_BYTES
    return (size + 31) & ~31


def emit_c(
    remap_rows: list[tuple[int, int, str]],
    by_id_rows: list[tuple[int, int, str]],
    overworld_rows: list[tuple[int, int, int, str]],
    impacts: list[dict],
    cannons: list[dict],
    bullets: list[dict],
) -> str:
    custom_count = len(impacts)
    cannon_count = len(cannons)
    bullet_count = len(bullets)
    impact_slots = VANILLA_IMPACT_COUNT + custom_count
    cannon_slots = VANILLA_CANNON_COUNT + cannon_count
    bullet_slots = VANILLA_BULLET_COUNT + bullet_count
    anm_max = gun_icon_anm_ext_max(custom_count + cannon_count + bullet_count)

    lines = [
        "/* Auto-generated by tools/compile_data_structures.py — do not edit. */",
        "",
        '#include "runtime.h"',
        '#include "data_structures.h"',
        '#include "overworld_enemy_ids.h"',
        "",
        "APPEND_RODATA const EnemyExpRemapEntry gEnemyExpRemap[] = {",
    ]
    for key, exp, label in remap_rows:
        lines.append(f"    {{ {key}, {exp} }}, /* {label} */")
    if not remap_rows:
        lines.append("    { 0, 0 },")
    lines.append("};")
    lines.append(f"APPEND_RODATA const u16 gEnemyExpRemapCount = {len(remap_rows)};")
    lines.append("")
    lines.append("APPEND_RODATA const EnemyExpIdEntry gEnemyExpById[] = {")
    for eid, exp, label in by_id_rows:
        lines.append(f"    {{ {eid}, {exp} }}, /* {label} */")
    if not by_id_rows:
        lines.append("    { 0, 0 },")
    lines.append("};")
    lines.append(f"APPEND_RODATA const u16 gEnemyExpByIdCount = {len(by_id_rows)};")
    lines.append("")
    lines.append("APPEND_RODATA const OverworldEnemyExpEntry gOverworldEnemyExp[] = {")
    for type_sym, anim_sym, eid, anim, exp, label in overworld_rows:
        lines.append(
            f"    {{ {type_sym}, {anim_sym}, {exp}, 0 }}, "
            f"/* {label} ({eid}/{anim}) */"
        )
    if not overworld_rows:
        lines.append("    { 0, 0, 0, 0 },")
    lines.append("};")
    lines.append(
        f"APPEND_RODATA const u16 gOverworldEnemyExpCount = {len(overworld_rows)};"
    )
    lines.append("")
    lines.append("APPEND_RODATA const CustomImpactEntry gCustomImpacts[] = {")
    if not impacts:
        lines.append("    { 0, 0, 0, 0, { 0 }, { 0, 0 } },")
    for row in impacts:
        desc_bytes = c_string_bytes(row["desc"], IMPACT_DESC_STRIDE)
        lines.append("    {")
        lines.append(
            f"        {row['index']}, {row['id']}, {row['number']}, {row['icon_from']},"
        )
        lines.append(f"        {{ {desc_bytes} }},")
        lines.append("        { 0, 0 }")
        lines.append(f"    }}, /* {row['label']} */")
    lines.append("};")
    lines.append(f"APPEND_RODATA const u16 gCustomImpactCount = {custom_count};")
    lines.append("")
    lines.append(
        f"/* impact_data.json → {custom_count} custom; "
        f"tables sized for {impact_slots} impacts (filled by apply_lynjump). */"
    )
    lines.append(f"APPEND_DATA u32 gImpactJumpTable[{impact_slots}] = {{0}};")
    lines.append(
        f"APPEND_DATA u8 gImpactDescTable[{impact_slots} * IMPACT_DESC_STRIDE] = {{0}};"
    )
    lines.append("")
    lines.append("APPEND_RODATA const CustomCannonEntry gCustomCannons[] = {")
    if not cannons:
        lines.append("    { 0, 0, 0, 0, 0, { 0 }, { 0, 0, 0 } },")
    for row in cannons:
        desc_bytes = c_string_bytes(row["desc"], CANNON_DESC_STRIDE)
        lines.append("    {")
        lines.append(
            f"        {row['index']}, {row['id']}, {row['number']}, "
            f"{row['icon_from']}, {row['fire_from']},"
        )
        lines.append(f"        {{ {desc_bytes} }},")
        lines.append("        { 0, 0, 0 }")
        lines.append(f"    }}, /* {row['label']} */")
    lines.append("};")
    lines.append(f"APPEND_RODATA const u16 gCustomCannonCount = {cannon_count};")
    lines.append("")
    lines.append(
        f"/* cannon_data.json → {cannon_count} custom; "
        f"tables sized for {cannon_slots} cannons (filled by apply_lynjump). */"
    )
    lines.append(f"APPEND_DATA u32 gCannonJumpTable[{cannon_slots}] = {{0}};")
    lines.append(
        f"APPEND_DATA u8 gCannonDescTable[{cannon_slots} * CANNON_DESC_STRIDE] = {{0}};"
    )
    lines.append("")
    lines.append("APPEND_RODATA const CustomBulletEntry gCustomBullets[] = {")
    if not bullets:
        lines.append("    { 0, 0, 0, 0, 0, { 0 }, { 0 } },")
    for row in bullets:
        desc_bytes = c_string_bytes(row["desc"], BULLET_DESC_STRIDE)
        lines.append("    {")
        lines.append(
            f"        {row['index']}, {row['id']}, {row['number']}, "
            f"{row['icon_from']}, {row['shot_from']},"
        )
        lines.append(f"        {{ {desc_bytes} }},")
        lines.append("        { 0 }")
        lines.append(f"    }}, /* {row['label']} */")
    lines.append("};")
    lines.append(f"APPEND_RODATA const u16 gCustomBulletCount = {bullet_count};")
    lines.append("")
    lines.append(
        f"/* bullet_data.json → {bullet_count} custom; "
        f"tables sized for {bullet_slots} bullets (filled by apply_lynjump). */"
    )
    lines.append(f"APPEND_DATA u32 gBulletJumpTable[{bullet_slots}] = {{0}};")
    lines.append(
        f"APPEND_DATA u8 gBulletDescTable[{bullet_slots} * BULLET_DESC_STRIDE] = {{0}};"
    )
    lines.append("")
    lines.append(
        f"/* Gun-icon ANM extension ({VANILLA_GUN_ICON_FRAMES}"
        f"+{custom_count * 2} impact+{cannon_count * 2} cannon"
        f"+{bullet_count * 2} bullet frames). */"
    )
    lines.append(f"#define GUN_ICON_ANM_EXT_MAX {anm_max}")
    lines.append(
        "APPEND_DATA u8 gGunIconAnmExt[GUN_ICON_ANM_EXT_MAX] "
        "__attribute__((aligned(4))) = {0};"
    )
    lines.append("APPEND_DATA u32 gGunIconAnmExtSize = 0;")
    lines.append("")
    return "\n".join(lines) + "\n"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--src",
        type=Path,
        default=REPO / "src_custom" / "data_structures",
        help="Directory of JSON data structure files",
    )
    parser.add_argument(
        "--out",
        type=Path,
        required=True,
        help="Output data_structures_tables.c path",
    )
    args = parser.parse_args()

    enemy_path = args.src / "enemy_exp.json"
    overworld_path = args.src / "overworld_enemy_exp.json"
    impact_path = args.src / "impact_data.json"
    cannon_path = args.src / "cannon_data.json"
    bullet_path = args.src / "bullet_data.json"
    for path in (enemy_path, overworld_path, impact_path, cannon_path, bullet_path):
        if not path.is_file():
            print(f"error: missing {path}", file=sys.stderr)
            return 1

    try:
        data = load_enemy_exp(enemy_path)
        remap_rows, by_id_rows = compile_enemy_exp(data)
        overworld_rows = load_overworld_enemy_exp(overworld_path)
        impacts = load_custom_impacts(impact_path)
        cannons = load_custom_cannons(cannon_path)
        bullets, bullet_overrides = load_custom_bullets(bullet_path)
    except (OSError, ValueError, json.JSONDecodeError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1

    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(
        emit_c(remap_rows, by_id_rows, overworld_rows, impacts, cannons, bullets)
    )
    print(
        f"data_structures: enemy_exp → {len(remap_rows)} remap, "
        f"{len(by_id_rows)} by-id; overworld_enemy_exp → {len(overworld_rows)}; "
        f"custom impacts → {len(impacts)}, "
        f"custom cannons → {len(cannons)}, custom bullets → {len(bullets)}, "
        f"bullet overrides → {len(bullet_overrides)}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

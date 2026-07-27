#!/usr/bin/env python3
"""Compile EVENT_SCRIPT choreography into JSON + opcode tables for the runner.

Emits:
  build/sss/event_scripts.json  — catalog (scene editor)
  build/sss/event_scripts.c     — APPEND_RODATA EventOp streams + gEventScriptTable

Talk bytes still come from compile_dialogue.py.
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[1]

SCRIPT_RE = re.compile(
    r"EVENT_SCRIPT_REPLACEMENT\s*\(\s*(0x[0-9A-Fa-f]+)\s*,\s*(\w+)"
    r"[^)]*\)"
    r"(.*?)"
    r"END_EVENT_SCRIPT\s*\(\s*\)",
    re.DOTALL,
)

CHOREO_RE = re.compile(
    r"\b("
    r"WAIT_COUNTER_GT|WAIT_SCROLL_Y_LE|WAIT_SCROLL_Y_GT|"
    r"WAIT_SCROLL_X_LE|WAIT_SCROLL_X_GT|WAIT_VEL_LE|WAIT_VEL_GT|DELAY|"
    r"SET_SCROLL_VEL|SET_SCROLL_VEL_X|SET_SCROLL_VEL_Y|"
    r"ADD_SCROLL_VEL_X|ADD_SCROLL_VEL_Y|CLAMP_SCROLL_VEL_X|CLAMP_SCROLL_VEL_Y|"
    r"SET_CUTSCENE_STEP|SCROLL_WORLD_Y|SCROLL_WORLD_Y_CAM2|"
    r"MOVE_FLYING_ACTORS_X|MOVE_FLYING_ACTORS_XY|SET_ACTOR_CTRL|"
    r"SHOW_ACTOR|HIDE_ACTOR|DELETE_ACTOR|SPAWN_ACTOR|MOVE_ACTOR|"
    r"SET_ACTOR_VEL_X|SET_ACTOR_VEL_Y|WALK_ACTOR_DIR|WALK_ACTOR|"
    r"CUTSCENE_TICK_A|CUTSCENE_TICK_B|CUTSCENE_FX|FADE_SCREEN|"
    r"PALETTE_AND|WAIT_RAND_CLEAR|SCROLL_VEL_FROM_DPAD|APPLY_CAMERA_DELTA|"
    r"PLAY_BGM|STOP_BGM|PLAY_VOICE|PLAY_VOICE_PAN|STOP_VOICE|"
    r"FALLTHROUGH|END|TALK|TEXT|CHAPTER_TITLE|CHOICE|EMPTY"
    r")\s*\(",
)

OP_ENUM = {
    "END": 0,
    "SET_CUTSCENE_STEP": 1,
    "WAIT_COUNTER_GT": 2,
    "WAIT_SCROLL_Y_GT": 3,
    "WAIT_SCROLL_Y_LE": 4,
    "WAIT_SCROLL_X_GT": 5,
    "WAIT_SCROLL_X_LE": 6,
    "WAIT_VEL_GT": 7,
    "WAIT_VEL_LE": 8,
    "SET_SCROLL_VEL": 9,
    "SET_SCROLL_VEL_X": 10,
    "SET_SCROLL_VEL_Y": 11,
    "ADD_SCROLL_VEL_X": 12,
    "ADD_SCROLL_VEL_Y": 13,
    "CLAMP_SCROLL_VEL_X": 14,
    "CLAMP_SCROLL_VEL_Y": 15,
    "SCROLL_WORLD_Y": 16,
    "SCROLL_WORLD_Y_CAM2": 17,
    "MOVE_FLYING_ACTORS_X": 18,
    "MOVE_FLYING_ACTORS_XY": 19,
    "SET_ACTOR_CTRL": 20,
    "CUTSCENE_TICK_A": 21,
    "CUTSCENE_TICK_B": 22,
    "CUTSCENE_FX": 23,
    "FADE_SCREEN": 24,
    "PALETTE_AND": 25,
    "WAIT_RAND_CLEAR": 26,
    "SCROLL_VEL_FROM_DPAD": 27,
    "APPLY_CAMERA_DELTA": 28,
    "TALK": 29,
    "MARK_STEP": 30,
    "SHOW_ACTOR": 31,
    "HIDE_ACTOR": 32,
    "DELETE_ACTOR": 33,
    "SPAWN_ACTOR": 34,
    "SET_ACTOR_VEL_X": 35,
    "SET_ACTOR_VEL_Y": 36,
    "WALK_ACTOR_DIR": 37,
    "MOVE_ACTOR": 38,
    "WALK_ACTOR": 39,
    "PLAY_BGM": 40,
    "STOP_BGM": 41,
    "PLAY_VOICE": 42,
    "PLAY_VOICE_PAN": 42,  # same op; pan in arg1
    "STOP_VOICE": 43,
}

KNOWN_CUTSCENE_SCENES = {
    "scene_05C9E1": {"fsm": "CutsceneCh1Opener", "rom": 0x080523EC, "talk_id": 51},
    "scene_0605BB": {"fsm": "CutsceneCh1TargetBrief", "rom": 0x0802BFA0, "talk_id": 109},
    "scene_05BF74": {"fsm": "BeatChain_1BE74", "rom": 0x0801CA0A, "talk_id": 0},
    "scene_05C005": {"fsm": "BeatChain_1BE74", "rom": 0x0801C18A, "talk_id": 6},
    "scene_05C035": {"fsm": "JtFsm_25394", "rom": 0x08025514, "talk_id": 8},
    "scene_05C05B": {"fsm": "JtFsm_25394", "rom": 0x080257C8, "talk_id": 9},
    "scene_0632E9": {"fsm": "StartTalk_4EA64", "rom": 0x0804EA64, "talk_id": 150},
    "scene_067DF7": {"fsm": "StartTalk_49F8C", "rom": 0x08049F8C, "talk_id": 205},
    "scene_068E30": {"fsm": "StartTalk_4D9F0", "rom": 0x0804D9F0, "talk_id": 226},
    "scene_071E72": {"fsm": "Fsm_49454", "rom": 0x08049454, "talk_id": 375},
    "scene_0741AA": {"fsm": "StartTalk_49E3E", "rom": 0x08049E3E, "talk_id": 409},
    "scene_077047": {"fsm": "StageCase175", "rom": 0x0802B648, "talk_id": 451},
    "scene_077146": {"fsm": "StartTalk_4D37C", "rom": 0x0804D37C, "talk_id": 453},
    "scene_07724F": {"fsm": "MapInteract_0D3FA", "rom": 0x0800D3FA, "talk_id": 455},
}


def parse_int(expr: str) -> int:
    expr = expr.strip().rstrip("uU")
    if not expr:
        return 0
    dir_consts = {
        "ACTOR_DIR_RIGHT": 0,
        "ACTOR_DIR_DOWN": 1,
        "ACTOR_DIR_LEFT": 2,
        "ACTOR_DIR_UP": 3,
    }
    if expr in dir_consts:
        return dir_consts[expr]
    # Evaluate simple shifts / arithmetic used in scene files.
    try:
        return int(expr, 0)
    except ValueError:
        pass
    safe = re.sub(r"[^0-9a-fxA-FX+\-<>|()~ ]", "", expr)
    try:
        return int(eval(safe, {"__builtins__": {}}, {}))  # noqa: S307
    except Exception as exc:  # noqa: BLE001
        raise ValueError(f"bad int expr: {expr!r}") from exc


def parse_args_list(body: str, start: int) -> tuple[list[str], int]:
    depth = 1
    i = start
    args: list[str] = []
    cur: list[str] = []
    in_str = False
    escape = False
    while i < len(body) and depth:
        ch = body[i]
        if in_str:
            cur.append(ch)
            if escape:
                escape = False
            elif ch == "\\":
                escape = True
            elif ch == '"':
                in_str = False
            i += 1
            continue
        if ch == '"':
            in_str = True
            cur.append(ch)
        elif ch == "(":
            depth += 1
            cur.append(ch)
        elif ch == ")":
            depth -= 1
            if depth == 0:
                arg = "".join(cur).strip()
                if arg:
                    args.append(arg)
                i += 1
                break
            cur.append(ch)
        elif ch == "," and depth == 1:
            args.append("".join(cur).strip())
            cur = []
        else:
            cur.append(ch)
        i += 1
    return args, i


def encode_op(name: str, args: list[str], talk_id: int | None) -> dict | None:
    if name in ("TEXT", "CHAPTER_TITLE", "CHOICE", "EMPTY", "FALLTHROUGH", "DELAY"):
        return None
    if name == "END":
        return {"op": OP_ENUM["END"], "arg0": 0, "arg1": 0, "name": name}
    if name == "TALK":
        tid = talk_id if talk_id is not None else 0
        return {
            "op": OP_ENUM["TALK"],
            "arg0": tid,
            "arg1": 4 | (3 << 8),
            "name": name,
            "pages": sum(1 for a in args[3:] if a.startswith('"')),
        }
    if name == "SET_CUTSCENE_STEP":
        return {
            "op": OP_ENUM["SET_CUTSCENE_STEP"],
            "arg0": parse_int(args[0]) if args else 0,
            "arg1": 0,
            "name": name,
        }
    if name in (
        "CUTSCENE_TICK_A",
        "CUTSCENE_TICK_B",
        "CUTSCENE_FX",
        "SCROLL_VEL_FROM_DPAD",
        "APPLY_CAMERA_DELTA",
    ):
        return {"op": OP_ENUM[name], "arg0": 0, "arg1": 0, "name": name}
    if name == "SET_SCROLL_VEL":
        return {
            "op": OP_ENUM[name],
            "arg0": parse_int(args[0]) if args else 0,
            "arg1": parse_int(args[1]) if len(args) > 1 else 0,
            "name": name,
        }
    if name == "FADE_SCREEN":
        a = [parse_int(x) if x else 0 for x in (args + ["0", "0", "0", "0"])[:4]]
        packed = (a[0] & 0xFF) | ((a[1] & 0xFF) << 8) | ((a[2] & 0xFF) << 16) | (
            (a[3] & 0xFF) << 24
        )
        return {"op": OP_ENUM[name], "arg0": packed, "arg1": 0, "name": name}
    if name == "WAIT_VEL_GT" or name == "WAIT_VEL_LE":
        return {
            "op": OP_ENUM[name],
            "arg0": parse_int(args[0]) if args else 0,
            "arg1": parse_int(args[1]) if len(args) > 1 else 0,
            "name": name,
        }
    if name == "MOVE_FLYING_ACTORS_XY":
        return {
            "op": OP_ENUM[name],
            "arg0": parse_int(args[0]) if args else 0,
            "arg1": parse_int(args[1]) if len(args) > 1 else 0,
            "name": name,
        }
    if name == "SET_ACTOR_CTRL":
        return {
            "op": OP_ENUM[name],
            "arg0": parse_int(args[0]) if args else 0,
            "arg1": parse_int(args[1]) if len(args) > 1 else 0,
            "name": name,
        }
    if name in ("HIDE_ACTOR", "DELETE_ACTOR"):
        return {
            "op": OP_ENUM[name],
            "arg0": parse_int(args[0]) if args else 0,
            "arg1": 0,
            "name": name,
        }
    if name in ("SHOW_ACTOR", "MOVE_ACTOR"):
        # arg0 = index; arg1 = x|(y<<16) — u16 world units
        index = parse_int(args[0]) if args else 0
        x = parse_int(args[1]) if len(args) > 1 else 0
        y = parse_int(args[2]) if len(args) > 2 else 0
        return {
            "op": OP_ENUM[name],
            "arg0": index & 0xFF,
            "arg1": (x & 0xFFFF) | ((y & 0xFFFF) << 16),
            "name": name,
        }
    if name == "SPAWN_ACTOR":
        # arg0: camera|(anim<<8)|(type<<24); arg1: x|(y<<16). Defaults flags.
        camera = parse_int(args[0]) if args else 0
        anim = parse_int(args[1]) if len(args) > 1 else 0
        typ = parse_int(args[2]) if len(args) > 2 else 0
        x = parse_int(args[3]) if len(args) > 3 else 0
        y = parse_int(args[4]) if len(args) > 4 else 0
        return {
            "op": OP_ENUM[name],
            "arg0": (camera & 0xFF) | ((anim & 0xFFFF) << 8) | ((typ & 0xFF) << 24),
            "arg1": (x & 0xFFFF) | ((y & 0xFFFF) << 16),
            "name": name,
        }
    if name in ("SET_ACTOR_VEL_X", "SET_ACTOR_VEL_Y"):
        return {
            "op": OP_ENUM[name],
            "arg0": parse_int(args[0]) if args else 0,
            "arg1": parse_int(args[1]) if len(args) > 1 else 0,
            "name": name,
        }
    if name == "WALK_ACTOR":
        # arg0 = index|(speed & ~0xFF); arg1 = x|(y<<16)
        index = parse_int(args[0]) if args else 0
        x = parse_int(args[1]) if len(args) > 1 else 0
        y = parse_int(args[2]) if len(args) > 2 else 0
        speed = parse_int(args[3]) if len(args) > 3 else 0x10000
        return {
            "op": OP_ENUM[name],
            "arg0": (index & 0xFF) | (speed & ~0xFF),
            "arg1": (x & 0xFFFF) | ((y & 0xFFFF) << 16),
            "name": name,
        }
    if name == "WALK_ACTOR_DIR":
        # arg0: index|(dir<<8); arg1: speed (16.16)
        index = parse_int(args[0]) if args else 0
        direction = parse_int(args[1]) if len(args) > 1 else 0
        speed = parse_int(args[2]) if len(args) > 2 else 0
        return {
            "op": OP_ENUM[name],
            "arg0": (index & 0xFF) | ((direction & 0xFF) << 8),
            "arg1": speed,
            "name": name,
        }
    if name == "PLAY_BGM":
        return {
            "op": OP_ENUM[name],
            "arg0": parse_int(args[0]) if args else 0,
            "arg1": 0,
            "name": name,
        }
    if name in ("STOP_BGM", "STOP_VOICE"):
        return {"op": OP_ENUM[name], "arg0": 0, "arg1": 0, "name": name}
    if name == "PLAY_VOICE":
        return {
            "op": OP_ENUM[name],
            "arg0": parse_int(args[0]) if args else 0,
            "arg1": 3,  # default pan (matches PlaySfx death call)
            "name": name,
        }
    if name == "PLAY_VOICE_PAN":
        return {
            "op": OP_ENUM["PLAY_VOICE"],
            "arg0": parse_int(args[0]) if args else 0,
            "arg1": parse_int(args[1]) if len(args) > 1 else 3,
            "name": name,
        }
    # unary numeric
    if name in OP_ENUM:
        return {
            "op": OP_ENUM[name],
            "arg0": parse_int(args[0]) if args else 0,
            "arg1": 0,
            "name": name,
        }
    return None


def parse_scene(path: Path) -> dict | None:
    text = path.read_text(encoding="utf-8")
    m = SCRIPT_RE.search(text)
    if not m:
        return None
    addr = int(m.group(1), 16)
    name = m.group(2)
    body = m.group(3)
    meta = KNOWN_CUTSCENE_SCENES.get(name, {})
    talk_id = meta.get("talk_id")
    ops = []
    for cm in CHOREO_RE.finditer(body):
        op_name = cm.group(1)
        args, _ = parse_args_list(body, cm.end())
        enc = encode_op(op_name, args, talk_id)
        if enc is None:
            if op_name == "TALK":
                ops.append(
                    {
                        "op": "TALK",
                        "speaker": args[0] if args else None,
                        "pages": sum(1 for a in args[3:] if a.startswith('"')),
                    }
                )
            continue
        ops.append(enc)
    bytecode = [o for o in ops if isinstance(o.get("op"), int)]
    # END alone is not choreography — map talks use END() as a terminator.
    move_ops = [o for o in bytecode if o["name"] not in ("TALK", "END")]
    return {
        "file": str(path.relative_to(REPO)),
        "name": name,
        "rom_addr": addr,
        "ops": ops,
        "bytecode": bytecode,
        "choreography_ops": len(move_ops),
        "fsm": meta.get("fsm"),
        "talk_id": talk_id,
        "emit_runner": bool(meta.get("fsm")) or len(move_ops) > 0,
    }


def collect_scenes(roots: list[Path]) -> list[dict]:
    scenes = []
    for root in roots:
        if not root.is_dir():
            continue
        for path in sorted(root.rglob("scene_*.c")):
            sc = parse_scene(path)
            if sc:
                scenes.append(sc)
    return scenes


def emit_c(scenes: list[dict]) -> str:
    # Only emit runner tables for cutscene-bearing scripts (known FSM or
    # non-talk choreography). Talk-only NPC scenes stay dialogue-only.
    cut = [s for s in scenes if s.get("emit_runner")]
    lines = [
        "/* Auto-generated by tools/compile_events.py — do not edit. */",
        '#include "gba/types.h"',
        '#include "event_runner.h"',
        '#include "runtime.h"',
        "",
    ]
    for s in cut:
        sym = f"gEventOps_{s['name']}"
        lines.append(f"APPEND_RODATA const EventOp {sym}[] = {{")
        for op in s["bytecode"]:
            a0 = op["arg0"] & 0xFFFFFFFF
            a1 = op["arg1"] & 0xFFFFFFFF
            # signed cast for C
            def s32(v: int) -> str:
                if v >= 0x80000000:
                    v = v - 0x100000000
                return str(v)

            lines.append(
                f"    {{ {op['op']}, {s32(a0)}, {s32(a1)} }}, /* {op['name']} */"
            )
        lines.append(f"    {{ {OP_ENUM['END']}, 0, 0 }},")
        lines.append("};")
        lines.append("")

    lines.append(f"APPEND_RODATA const u32 gEventScriptCount = {len(cut)};")
    lines.append("")
    lines.append("APPEND_RODATA const EventScript gEventScriptTable[] = {")
    for s in cut:
        tid = s.get("talk_id") if s.get("talk_id") is not None else 0xFFFF
        nops = len(s["bytecode"]) + 1  # include END
        lines.append(
            f'    {{ "{s["name"]}", {tid}, {nops}, gEventOps_{s["name"]} }},'
        )
    if not cut:
        lines.append('    { "", 0xFFFF, 0, 0 },')
    lines.append("};")
    lines.append("")
    return "\n".join(lines)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--src", action="append", type=Path)
    ap.add_argument(
        "--out-json",
        type=Path,
        default=REPO / "build" / "sss" / "event_scripts.json",
    )
    ap.add_argument(
        "--out-c",
        type=Path,
        default=REPO / "build" / "sss" / "event_scripts.c",
    )
    ap.add_argument("--check", action="store_true")
    args = ap.parse_args()

    roots = args.src or [
        REPO / "src_custom" / "dialogue",
        REPO / "events" / "scripts",
    ]
    scenes = collect_scenes(roots)
    if not scenes:
        print("compile_events: no EVENT_SCRIPT scenes found", file=sys.stderr)
        return 1

    choreo = [s for s in scenes if s.get("emit_runner")]
    print(f"compile_events: {len(scenes)} scenes, {len(choreo)} runner tables")
    for s in choreo:
        print(f"  {s['name']}: {s['choreography_ops']} ops → {s.get('fsm') or '?'}")

    if args.check:
        return 0

    args.out_json.parent.mkdir(parents=True, exist_ok=True)
    # JSON without huge redundancy
    json_scenes = []
    for s in scenes:
        json_scenes.append(
            {
                "file": s["file"],
                "name": s["name"],
                "rom_addr": s["rom_addr"],
                "fsm": s.get("fsm"),
                "talk_id": s.get("talk_id"),
                "choreography_ops": s["choreography_ops"],
                "ops": s["ops"],
            }
        )
    args.out_json.write_text(json.dumps(json_scenes, indent=2) + "\n", encoding="utf-8")
    args.out_c.write_text(emit_c(scenes), encoding="utf-8")
    print(f"wrote {args.out_json}")
    print(f"wrote {args.out_c}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

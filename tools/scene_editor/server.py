#!/usr/bin/env python3
"""Local scene editor HTTP server — list / read / write dialogue scene .c files.

  python3 tools/scene_editor/server.py
  open http://127.0.0.1:8765/
"""

from __future__ import annotations

import json
import re
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import parse_qs, urlparse

REPO = Path(__file__).resolve().parents[2]
DIALOGUE = REPO / "src_custom" / "dialogue"
STATIC = Path(__file__).resolve().parent
CATALOG = REPO / "build" / "sss" / "event_scripts.json"

SCENE_NAME_RE = re.compile(r"^scene_[0-9A-Fa-f]+\.c$")
SAFE_REL = re.compile(r"^chapter_[0-9a-z_]+/scene_[0-9A-Fa-f]+\.c$")


def list_scenes() -> list[dict]:
    out = []
    for path in sorted(DIALOGUE.rglob("scene_*.c")):
        rel = path.relative_to(DIALOGUE).as_posix()
        out.append({"file": f"src_custom/dialogue/{rel}", "rel": rel, "name": path.stem})
    return out


class Handler(BaseHTTPRequestHandler):
    def _cors(self):
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, PUT, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")

    def _json(self, code: int, obj):
        data = json.dumps(obj).encode("utf-8")
        self.send_response(code)
        self.send_header("Content-Type", "application/json")
        self._cors()
        self.send_header("Content-Length", str(len(data)))
        self.end_headers()
        self.wfile.write(data)

    def _bytes(self, code: int, data: bytes, ctype: str):
        self.send_response(code)
        self.send_header("Content-Type", ctype)
        self._cors()
        self.send_header("Content-Length", str(len(data)))
        self.end_headers()
        self.wfile.write(data)

    def do_OPTIONS(self):
        self.send_response(204)
        self._cors()
        self.end_headers()

    def do_GET(self):
        parsed = urlparse(self.path)
        path = parsed.path
        if path in ("/", "/index.html"):
            self._bytes(200, (STATIC / "index.html").read_bytes(), "text/html; charset=utf-8")
            return
        if path == "/api/scenes":
            self._json(200, {"scenes": list_scenes()})
            return
        if path == "/api/catalog":
            if CATALOG.is_file():
                self._bytes(200, CATALOG.read_bytes(), "application/json")
            else:
                self._json(404, {"error": "run compile_events.py first"})
            return
        if path == "/api/scene":
            qs = parse_qs(parsed.query)
            rel = qs.get("path", [""])[0]
            if not SAFE_REL.match(rel):
                self._json(400, {"error": "bad path"})
                return
            fp = DIALOGUE / rel
            if not fp.is_file():
                self._json(404, {"error": "not found"})
                return
            self._json(200, {"path": rel, "text": fp.read_text(encoding="utf-8")})
            return
        self._json(404, {"error": "not found"})

    def do_PUT(self):
        parsed = urlparse(self.path)
        if parsed.path != "/api/scene":
            self._json(404, {"error": "not found"})
            return
        length = int(self.headers.get("Content-Length", "0"))
        body = json.loads(self.rfile.read(length).decode("utf-8"))
        rel = body.get("path", "")
        text = body.get("text")
        if not SAFE_REL.match(rel) or not isinstance(text, str):
            self._json(400, {"error": "bad body"})
            return
        if "EVENT_SCRIPT" not in text and "DIALOGUE_SCRIPT" not in text:
            self._json(400, {"error": "refusing write without SCRIPT wrapper"})
            return
        fp = DIALOGUE / rel
        if not fp.is_file():
            self._json(404, {"error": "not found"})
            return
        fp.write_text(text, encoding="utf-8")
        self._json(200, {"ok": True, "path": rel, "bytes": len(text.encode("utf-8"))})

    def log_message(self, fmt, *args):
        print(f"[scene_editor] {self.address_string()} {fmt % args}")


def main() -> int:
    host, port = "127.0.0.1", 8765
    httpd = ThreadingHTTPServer((host, port), Handler)
    print(f"scene editor: http://{host}:{port}/")
    print(f"dialogue root: {DIALOGUE}")
    httpd.serve_forever()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

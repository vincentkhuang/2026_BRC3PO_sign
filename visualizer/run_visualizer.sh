#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
BUILD_DIR="$SCRIPT_DIR/.build"
GENERATED_DIR="$SCRIPT_DIR/generated"
PORT="${PORT:-8765}"
SEED="${SEED:-42}"
OPEN_BROWSER=1

if [[ "${1:-}" == "--no-open" ]]; then
  OPEN_BROWSER=0
elif [[ -n "${1:-}" ]]; then
  echo "Usage: $0 [--no-open]" >&2
  exit 2
fi

if [[ -n "${CXX:-}" ]]; then
  CXX_BIN="$CXX"
elif command -v clang++ >/dev/null 2>&1; then
  CXX_BIN="clang++"
elif command -v c++ >/dev/null 2>&1; then
  CXX_BIN="c++"
else
  echo "A C++17 compiler is required (clang++ or c++)." >&2
  exit 1
fi

if ! command -v python3 >/dev/null 2>&1; then
  echo "python3 is required to serve the visualizer." >&2
  exit 1
fi

mkdir -p "$BUILD_DIR" "$GENERATED_DIR"

echo "Building the native pattern recorder…"
"$CXX_BIN" -std=c++17 -O2 \
  -I"$SCRIPT_DIR/sim" \
  -I"$ROOT_DIR" \
  "$SCRIPT_DIR/sim/FastLED.cpp" \
  "$SCRIPT_DIR/sim/main.cpp" \
  "$ROOT_DIR/src/core/LedHardware.cpp" \
  "$ROOT_DIR/src/core/SignTransitions.cpp" \
  "$ROOT_DIR/src/core/SignLayout.cpp" \
  "$ROOT_DIR/src/core/Controls.cpp" \
  "$ROOT_DIR/src/patterns/IdleScannerPattern.cpp" \
  "$ROOT_DIR/src/patterns/LetterPatterns.cpp" \
  "$ROOT_DIR/src/patterns/PalettePattern.cpp" \
  "$ROOT_DIR/src/patterns/DancePattern.cpp" \
  "$ROOT_DIR/src/patterns/ColorDropPattern.cpp" \
  "$ROOT_DIR/src/patterns/DiscoStrobePattern.cpp" \
  "$ROOT_DIR/src/shows/Shows.cpp" \
  "$ROOT_DIR/src/patterns/TypewriterPattern.cpp" \
  "$ROOT_DIR/src/patterns/SignalRelayPattern.cpp" \
  "$ROOT_DIR/src/patterns/ProtocolGoldPattern.cpp" \
  "$ROOT_DIR/src/patterns/LetterDominoPattern.cpp" \
  "$ROOT_DIR/src/patterns/PowerUpPattern.cpp" \
  "$ROOT_DIR/src/patterns/BlackoutRevealPattern.cpp" \
  "$ROOT_DIR/src/patterns/CometLettersPattern.cpp" \
  "$ROOT_DIR/src/patterns/FadeOutPattern.cpp" \
  "$ROOT_DIR/src/shows/NewPatternsShow.cpp" \
  -o "$BUILD_DIR/brc3po-sim"

PATTERNS=(
  show
  idle-scanner
  power-up
  typewriter
  signal-relay
  letter-domino
  comet-letters
  blackout-reveal
  protocol-gold
  fade-out
  legacy-first
  legacy-second
)

echo "Recording real C++ pattern frames with seed ${SEED}…"
for pattern in "${PATTERNS[@]}"; do
  "$BUILD_DIR/brc3po-sim" \
    "$pattern" \
    "$GENERATED_DIR/$pattern.bin" \
    "$SEED"
done

URL="http://127.0.0.1:$PORT/index.html"
echo "Serving $URL"
python3 -m http.server "$PORT" --bind 127.0.0.1 --directory "$SCRIPT_DIR" >/dev/null 2>&1 &
SERVER_PID=$!
trap 'kill "$SERVER_PID" >/dev/null 2>&1 || true' EXIT INT TERM

if [[ "$OPEN_BROWSER" -eq 1 ]]; then
  if [[ "$(uname -s)" == "Darwin" ]]; then
    open "$URL"
  elif command -v xdg-open >/dev/null 2>&1; then
    xdg-open "$URL" >/dev/null 2>&1 || true
  else
    echo "Open $URL in your browser."
  fi
else
  echo "Open $URL in your browser."
fi

echo "Press Ctrl-C to stop the local server."
wait "$SERVER_PID"

# BRC3PO LED Controller

Arduino/FastLED firmware and a local visualizer for the eight-channel BRC3PO sign. A Teensy and OctoWS2811 drive six block letters (`BRC3PO`), an underline, and a mailbox flag; two optional potentiometers control hue and brightness.

## Local visualizer

The visualizer is the easiest way to explore the project. It compiles the real pattern and show code against a simulated FastLED/Arduino layer, records every LED frame, and displays the sign in a browser.

### Run it

Requirements: macOS or Linux, Python 3, a C++17 compiler such as `clang++`, and Chrome 55 or newer. On a new Mac, install the compiler with `xcode-select --install`.

```sh
cd /path/to/2026_BRC3PO_sign
./visualizer/run_visualizer.sh
```

The loader rebuilds all recordings, starts `http://127.0.0.1:8765`, and opens it in your browser. Keep the Terminal window open and press `Ctrl-C` to stop it. Do not open `visualizer/index.html` directly—the browser cannot load recordings from a `file://` page.

### Use it

- Both pattern selectors are grouped into complete Shows and Individual Patterns. The individual list includes the four legacy building blocks: Palette, Dance, Color Drop, and Disco Strobe.
- **Show sequence** starts with the Teensy's exact six-part runtime order and loops endlessly by default.
- Add, reorder, remove, or repeat queue entries to audition other combinations.
- Playback speed and brightness affect only the preview, not the firmware.
- Idle Scanner is shortened to one 25.48-second out-and-back sweep instead of its four-minute live interval.

Optional loader settings:

```sh
./visualizer/run_visualizer.sh --no-open  # Build and serve without opening a browser
SEED=123 ./visualizer/run_visualizer.sh   # Try another coordinated-show shuffle
PORT=9000 ./visualizer/run_visualizer.sh  # Use another local port
CXX=g++ ./visualizer/run_visualizer.sh    # Select another C++ compiler
```

If frames do not load, confirm the address begins with `http://127.0.0.1` and rerun the loader. If the port is occupied, use the `PORT` option above. Generated recordings are ignored by Git and live in `visualizer/generated/`.

If Chrome reports `Uncaught SyntaxError: Unexpected token '.'` and the pattern menus remain empty, update to this version of the visualizer. Its browser script intentionally avoids optional chaining and `Element.replaceChildren()`, which are unavailable in older Chrome releases.

The drawing uses the measured strand and segment lengths. Its block-letter geometry remains a visual approximation; firmware LED counts, colors, timing, fades, and sequence logic are code-accurate.

## Live show sequence

The Teensy begins in Idle Scanner and repeats this sequence without manual input:

1. **Idle Scanner** — 4 minutes
2. **Coordinated Full Show**
3. **Idle Scanner** — 4 minutes
4. **Legacy First Show**
5. **Idle Scanner** — 4 minutes
6. **Legacy Second Show**

Each show receives a full four-minute Idle Scanner interval after the preceding show finishes. Change `IDLE_SCANNER_DURATION_MS` in `src/core/Config.h` to adjust it.

The coordinated show runs Power-Up, Typewriter, a shuffled pass through Signal Relay, Letter Domino, and Comet Letters, then Blackout Reveal, Protocol Gold, and Fade Out. Every shuffled pattern runs once, and the shuffle avoids repeating the same pattern at the boundary between cycles.

Legacy First combines Palette and Dance. Legacy Second combines white flashes, Color Drop, rotating letter colors, and Disco Strobe. Whole-sign light/dark changes use fades while intentional flashes, strobes, sweeps, and fills remain crisp.

## Hardware and controls

Required hardware:

- Teensy-compatible microcontroller and OctoWS2811 adapter
- Eight NeoPixel/WS2811-compatible strips
- Suitable external LED power supply with a common ground
- Optional hue and brightness potentiometers

The controller reserves 144 pixels on each of eight channels. The installed sign uses 894 LEDs:

| Element | Physical layout | LEDs | Buffer offset |
| --- | --- | ---: | ---: |
| `3` | Continuous | 87 | 0 |
| `C` | Continuous | 86 | 144 |
| `R` | 63 exterior + 51 interior | 114 | 288 |
| `B` | 60 exterior + 60 interior | 120 | 432 |
| Flag | Continuous | 138 | 576 |
| Underline | 68 side 1 + 72 side 2 | 140 | 720 |
| `O` | 62 exterior + 47 interior | 109 | 864 |
| `P` | Continuous | 100 | 1008 |

The color knob uses analog input 3, the brightness knob uses analog input 5, and pin 13 is the status LED. Physical wiring determines the channel offsets in `src/core/Config.h`.

### Disable either knob

In `src/core/Config.h`, change either switch to `false` and set its fixed replacement value:

```cpp
constexpr bool ENABLE_COLOR_KNOB = true;
constexpr bool ENABLE_BRIGHTNESS_KNOB = false;
constexpr uint8_t FIXED_LETTER_HUE = 0;  // 0-255
constexpr uint8_t FIXED_BRIGHTNESS = 255; // 0-255
```

The controller starts at FastLED's maximum brightness, `255`. The brightness knob is disabled, so control reads keep brightness at `FIXED_BRIGHTNESS`. Set `ENABLE_BRIGHTNESS_KNOB` to `true` to restore knob control; the next control read will then replace the startup value with the knob setting. FastLED hue examples are approximately `0` red, `85` green, and `170` blue. Power down the installation before disconnecting or rewiring a knob.

## Code layout

| Path | Purpose |
| --- | --- |
| `BRC3PO_2018_Master.ino` | Arduino `setup()` and `loop()` |
| `src/core/` | Configuration, hardware, controls, layout, and transitions |
| `src/patterns/` | Individual patterns and shared pattern helpers |
| `src/shows/` | Coordinated/legacy shows and the endless scheduler |
| `visualizer/` | Native recorder, simulation layer, browser UI, and recordings |

Every measured exterior and interior letter segment starts at its bottom-left corner. B, R, and O are stored exterior-first and interior-second in their channel buffers; C, 3, and P each use one continuous strand. Coordinated timing is centralized in `src/patterns/PatternTiming.h`; legacy show timing remains independently scaled in `src/shows/Shows.cpp`.

## Upload to the Teensy

1. Install Teensy support. Use the FastLED and OctoWS2811 copies bundled with the Teensy platform; separately installed copies can override the compatible bundled versions and produce duplicate-library warnings.
2. Keep `BRC3PO_2018_Master.ino` beside the complete `src/` directory and open the sketch.
3. Select the Teensy board and USB port, then confirm the configured strip counts, offsets, and inputs.
4. Compile and upload.

Use a properly sized external LED power supply; do not power the installation from the Teensy's USB connection. The visualizer validates software output, but final color, current draw, timing, and physical direction must be checked on the sign.

## License

No license has been selected yet.

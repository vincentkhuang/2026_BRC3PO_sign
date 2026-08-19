# BRC3PO LED Controller

Arduino/FastLED firmware for a large, eight-channel NeoPixel installation driven by a Teensy and an OctoWS2811 adapter.

The sketch controls six illuminated characters (`B`, `R`, `C`, `3`, `P`, and `O`), an underline, and a flag. Two potentiometers provide live hue and brightness control, while timed show modes add palette waves, dancing letters, falling colors, and a rainbow strobe.

## Hardware

- Teensy-compatible microcontroller
- OctoWS2811 adapter
- Eight NeoPixel/WS2811-compatible LED strips
- Two potentiometers for hue and brightness
- Appropriate external LED power supply and common ground

The controller allocates 144 pixels for each of eight output channels, for 1,152 buffer positions. The installation uses 934 LEDs:

| Element | LEDs | Channel offset |
| --- | ---: | ---: |
| `3` | 94 | 0 |
| `C` | 79 | 144 |
| `R` | 119 | 288 |
| `B` | 141 | 432 |
| Flag (`F`) | 143 | 576 |
| Underline (`U`) | 126 | 720 |
| `O` | 128 | 864 |
| `P` | 104 | 1008 |

The physical output order is determined by the control-box and patch-box connections. Update the offsets in the sketch if the installation is wired differently.

## Software requirements

- Arduino IDE or Arduino CLI with Teensy support
- [FastLED](https://github.com/FastLED/FastLED)
- [OctoWS2811](https://github.com/PaulStoffregen/OctoWS2811)

## Code organization

The sketch uses a small hardware layer plus one module per visual pattern:

| Module | Responsibility |
| --- | --- |
| `BRC3PO_2018_Master.ino` | Arduino `setup()` and `loop()` only |
| `Config.h` | Pin assignments, strip sizes, channel offsets, and timing constants |
| `TeensyController.*` | Serial startup and the Teensy status LED |
| `LedHardware.*` | OctoWS2811/FastLED initialization, LED buffer, and named strip views |
| `Controls.*` | Potentiometer sampling and the selected color/brightness state |
| `IdleScannerPattern.*` | Normal sign display and bouncing underline scanner |
| `PalettePattern.*` | Rainbow flag/underline and party-palette rendering |
| `DancePattern.*` | Alternating letter pixels and back-and-forth movement |
| `ColorDropPattern.*` | Falling colors on the flag |
| `DiscoStrobePattern.*` | Moving rainbow strobe dashes on the underline |
| `SignLayout.*` | Shared reading order, letter access, and six underline regions |
| `TypewriterPattern.*` | Reveals the letters in reading order with an underline cursor |
| `SignalRelayPattern.*` | Sends a charge from the flag through the underline and letters |
| `ProtocolGoldPattern.*` | Gold letter shimmer, sparkles, and breathing accents |
| `LetterDominoPattern.*` | Activates and clears letters forward and backward |
| `PowerUpPattern.*` | Flicker, progress bar, sequential lock-on, and final flash |
| `BlackoutRevealPattern.*` | Clockwise synchronized blackout and reveal |
| `CometLettersPattern.*` | Runs a colored comet clockwise around each letter |
| `FadeOutPattern.*` | Smoothly fades the entire installation to black between shows |
| `NewPatternsShow.*` | Sequences the eight new patterns as a single show |
| `LetterPatterns.*` | Shared letter fills, palette rotation, and shifting |
| `Shows.*` | First/second show sequencing and the ten-minute scheduler |

OctoWS2811 and FastLED intentionally share `LedHardware` because their setup is one hardware boundary: the Octo adapter determines how FastLED maps the single LED buffer onto eight physical channels.

## Controls

| Input | Analog channel | Purpose |
| --- | ---: | --- |
| Color potentiometer | 3 | Selects the letter hue |
| Brightness potentiometer | 5 | Sets global LED brightness, capped at 240/255 |

Pin 13 is used as a power/debug indicator.

### Disabling the control knobs

The knobs can be disabled independently in `Config.h`. This is useful when a knob is disconnected, faulty, or the installation should always start with a fixed appearance.

```cpp
constexpr bool ENABLE_COLOR_KNOB = true;
constexpr bool ENABLE_BRIGHTNESS_KNOB = true;
```

Change a knob's setting from `true` to `false` to stop reading its analog input. Then set the value that should be used instead:

```cpp
constexpr uint8_t FIXED_LETTER_HUE = 0;
constexpr uint8_t FIXED_BRIGHTNESS = 96;
```

- `FIXED_LETTER_HUE` accepts FastLED hue values from `0` to `255`. For example, `0` is red, approximately `85` is green, and approximately `170` is blue.
- `FIXED_BRIGHTNESS` accepts values from `0` to `240`, matching this project's configured brightness limit.
- To disable both knobs, set both `ENABLE_*_KNOB` values to `false`.

Turning a switch off in software means its physical analog input is ignored. Power down the installation before physically disconnecting or rewiring a knob.

## Show sequence

During normal operation, the letters use the selected potentiometer color, the flag is red, and a white scanner moves along the underline.

At startup, the controller runs both programmed shows:

1. **New Patterns Preview** — Power-Up, Typewriter, a shuffled kinetic section, Blackout Reveal, Protocol Gold, and Fade Out.
2. **First Show** — rainbow flag and underline gradients with party-palette letters, followed by a back-and-forth letter dance.
3. **Second Show** — white flashes, rotating rainbow letter colors, falling colors on the flag, and moving rainbow strobe dashes on the underline.

After startup, all three shows rotate on a ten-minute timer. To skip the new-pattern preview at power-up while keeping it in the timed rotation, set `RUN_NEW_PATTERN_PREVIEW_AT_STARTUP` to `false` in `Config.h`.

The letter strips are assumed to advance clockwise as their pixel indices increase. Blackout Reveal and Comet Letters begin at each strip's installed pixel `0`; no physical or software start-point alignment is required.

The new-pattern show uses a structured random sequence:

1. Power-Up
2. Typewriter
3. Signal Relay, Letter Domino, and Comet Letters in a shuffled order
4. Blackout Reveal
5. Protocol Gold
6. Fade Out

Every kinetic pattern plays exactly once per cycle. The shuffle also prevents the last kinetic pattern from one cycle from becoming the first kinetic pattern in the next cycle. Fade Out leaves the installation fully black so the next Power-Up has a clean starting point.

The coordinated patterns deliberately vary their color treatment: Power-Up and Blackout Reveal use full-sign rainbows, Typewriter uses a cool-shifted rainbow, Signal Relay and Letter Domino rotate colors as they travel, Comet Letters assigns a different rainbow hue to each letter, and Protocol Gold keeps its gold/white signature. Fade Out preserves whichever colors are already showing as it takes the sign to black.

All coordinated-pattern delays pass through `PatternTiming.h`, where `PATTERN_TIME_SCALE_PERCENT` is set to `110`. This makes the complete show 10% slower while carrying fractional milliseconds between short animation frames so their rounding does not change the overall speed.

## Uploading

1. Install Teensy support, FastLED, and OctoWS2811 in your Arduino environment.
2. Keep every `.ino`, `.h`, and `.cpp` file together in the sketch folder, then open `BRC3PO_2018_Master.ino`.
3. Select the Teensy board and the correct USB port.
4. Confirm the strip lengths, channel offsets, and potentiometer inputs match the hardware.
5. Compile and upload the sketch.

Use a properly sized external power supply for the LEDs. Do not power a large installation from the microcontroller's USB connection.

## Local pattern visualizer

The repository includes a code-accurate local visualizer that compiles the real C++ pattern modules against a simulated FastLED/Arduino layer. Every call to `LEDS.show()` is captured, then played over an approximate layout of the six letters, underline, and mailbox flag.

### Requirements

- macOS or Linux
- A C++17 compiler such as `clang++`
- Python 3

On a new macOS installation, running `xcode-select --install` installs the required command-line compiler. Linux users can use their distribution's normal C++ compiler and Python packages.

### Quick start

1. Open Terminal.
2. Change into the cloned repository directory.
3. Run the loader from the repository root:

```sh
cd /path/to/2026_BRC3PO_sign
./visualizer/run_visualizer.sh
```

The loader then:

1. Builds the native C++ pattern recorder.
2. Regenerates the complete show and every individual pattern from the current firmware source.
3. Starts a local server at `http://127.0.0.1:8765`.
4. Opens the visualizer in the default browser.

Keep that Terminal window open while using the visualizer. Press `Ctrl-C` there to stop the local server.

Do not open `visualizer/index.html` directly as a `file://` page. Browser security prevents a directly opened file from loading the generated pattern recordings; always use the `http://127.0.0.1:8765` address started by the loader.

### Player controls

- **Pattern** selects the structured full show or one individual pattern.
- **Play / Pause** and **Restart** control playback.
- **Timeline** scrubs to any recorded frame.
- **Playback speed** changes only local preview speed; it does not edit firmware timing.
- **Brightness** changes only the preview display level.
- **Strip start markers** show logical pixel `0` and the estimated start of each letter's interior run.

### Loader options

```sh
# Build and serve without opening a browser
./visualizer/run_visualizer.sh --no-open

# Try a different structured-shuffle order
SEED=123 ./visualizer/run_visualizer.sh

# Use another local port
PORT=9000 ./visualizer/run_visualizer.sh

# Select a specific C++ compiler
CXX=g++ ./visualizer/run_visualizer.sh
```

Each letter is drawn as a block-letter exterior plus an interior run, and the mailbox flag includes its vertical pole and square top, based on the installed sign. Exact LED counts and buffer offsets come from the firmware; the visualizer distributes each letter's pixels between its two paths in proportion to their drawn lengths because the physical per-strip counts and start coordinates are not yet recorded. Colors, timing, fades, and pattern sequencing come from the firmware itself.

### Troubleshooting

- **Could not load frames:** confirm the address begins with `http://127.0.0.1`, not `file://`, and rerun the loader.
- **Port already in use:** start it on another port, such as `PORT=9000 ./visualizer/run_visualizer.sh`.
- **Compiler or Python missing:** install the requirement named in the Terminal error, then run the loader again.
- **Firmware changed:** stop and rerun the loader so it recompiles the source and regenerates every recording.

## Project status

This is legacy 2018 firmware being preserved for the 2026 installation. Before deploying it unattended, review the blocking animation timing, strip-shift boundaries, and `ColorDrop` palette indexing. The local visualizer checks pattern output without hardware, but final timing, color, current draw, and physical LED direction still require testing on the sign.

## License

No license has been selected yet.

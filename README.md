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

1. **First Show** — rainbow flag and underline gradients with party-palette letters, followed by a back-and-forth letter dance.
2. **Second Show** — white flashes, rotating rainbow letter colors, falling colors on the flag, and moving rainbow strobe dashes on the underline.

After startup, the two shows alternate on a ten-minute timer.

## Uploading

1. Install Teensy support, FastLED, and OctoWS2811 in your Arduino environment.
2. Keep every `.ino`, `.h`, and `.cpp` file together in the sketch folder, then open `BRC3PO_2018_Master.ino`.
3. Select the Teensy board and the correct USB port.
4. Confirm the strip lengths, channel offsets, and potentiometer inputs match the hardware.
5. Compile and upload the sketch.

Use a properly sized external power supply for the LEDs. Do not power a large installation from the microcontroller's USB connection.

## Project status

This is legacy 2018 firmware being preserved for the 2026 installation. Before deploying it unattended, review the blocking animation timing, strip-shift boundaries, and `ColorDrop` palette indexing. Hardware testing is required because the repository does not currently include an automated simulator or test suite.

## License

No license has been selected yet.

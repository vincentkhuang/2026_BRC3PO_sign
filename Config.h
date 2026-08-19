#pragma once

#include <Arduino.h>

// Analog inputs
constexpr uint8_t POT_COLOR = 3;
constexpr uint8_t POT_BRIGHTNESS = 5;

// Set either switch to false to ignore that physical knob. The matching
// fixed value below is used whenever a knob is disabled.
constexpr bool ENABLE_COLOR_KNOB = true;
constexpr bool ENABLE_BRIGHTNESS_KNOB = true;
constexpr uint8_t FIXED_LETTER_HUE = 0;
constexpr uint8_t FIXED_BRIGHTNESS = 96;

// Run the seven-pattern preview immediately after power-up, before the two
// legacy shows. Set false once installation testing is complete if desired.
constexpr bool RUN_NEW_PATTERN_PREVIEW_AT_STARTUP = true;

// Teensy status LED
constexpr uint8_t POWER_PIN = 13;

// OctoWS2811 allocates the same length for all eight output channels.
constexpr int NUM_STRIPS = 8;
constexpr int NUM_LEDS_PER_STRIP = 144;
constexpr int NUM_LEDS = NUM_STRIPS * NUM_LEDS_PER_STRIP;

// Physical LED counts
constexpr int NUM_B = 141;
constexpr int NUM_R = 119;
constexpr int NUM_C = 79;
constexpr int NUM_3 = 94;
constexpr int NUM_P = 104;
constexpr int NUM_O = 128;
constexpr int NUM_U = 126;
constexpr int NUM_F = 143;

// Channel offsets determined by the control-box and patch-box wiring.
constexpr int THREE_START = 0;
constexpr int C_START = NUM_LEDS_PER_STRIP;
constexpr int R_START = 2 * NUM_LEDS_PER_STRIP;
constexpr int B_START = 3 * NUM_LEDS_PER_STRIP;
constexpr int F_START = 4 * NUM_LEDS_PER_STRIP;
constexpr int U_START = 5 * NUM_LEDS_PER_STRIP;
constexpr int O_START = 6 * NUM_LEDS_PER_STRIP;
constexpr int P_START = 7 * NUM_LEDS_PER_STRIP;

constexpr uint8_t INITIAL_BRIGHTNESS = 96;
constexpr uint8_t MAX_BRIGHTNESS = 240;
constexpr uint16_t IDLE_FRAME_LENGTH_MS = 100;

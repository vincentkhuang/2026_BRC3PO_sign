#pragma once

#include <Arduino.h>

// Analog inputs
constexpr uint8_t POT_COLOR = 3;
constexpr uint8_t POT_BRIGHTNESS = 5;

// Set either switch to false to ignore that physical knob. The matching
// fixed value below is used whenever a knob is disabled.
constexpr bool ENABLE_COLOR_KNOB = true;
constexpr bool ENABLE_BRIGHTNESS_KNOB = false;
constexpr uint8_t FIXED_LETTER_HUE = 0;
constexpr uint8_t FIXED_BRIGHTNESS = 140;

// Rest in Idle Scanner for this long after power-up and after every show.
// The scheduler then advances Coordinated -> Legacy First -> Legacy Second.
constexpr unsigned long IDLE_SCANNER_DURATION_MS = 7UL * 60UL * 1000UL;

// Teensy status LED
constexpr uint8_t POWER_PIN = 13;

// OctoWS2811 allocates the same length for all eight output channels.
constexpr int NUM_STRIPS = 8;
constexpr int NUM_LEDS_PER_STRIP = 144;
constexpr int NUM_LEDS = NUM_STRIPS * NUM_LEDS_PER_STRIP;

// Measured physical LED counts. The split letter segments are wired exterior
// first, then interior; both segments begin at their bottom-left corner.
constexpr int NUM_B_EXTERIOR = 60;
constexpr int NUM_B_INTERIOR = 60;
constexpr int NUM_B = NUM_B_EXTERIOR + NUM_B_INTERIOR;

constexpr int NUM_R_EXTERIOR = 63;
constexpr int NUM_R_INTERIOR = 51;
constexpr int NUM_R = NUM_R_EXTERIOR + NUM_R_INTERIOR;

constexpr int NUM_C = 86;
constexpr int NUM_3 = 87;
constexpr int NUM_P = 100;

constexpr int NUM_O_EXTERIOR = 62;
constexpr int NUM_O_INTERIOR = 47;
constexpr int NUM_O = NUM_O_EXTERIOR + NUM_O_INTERIOR;

constexpr int NUM_U_SIDE_1 = 68;
constexpr int NUM_U_SIDE_2 = 72;
constexpr int NUM_U = NUM_U_SIDE_1 + NUM_U_SIDE_2;
constexpr int NUM_F = 138;

static_assert(NUM_B <= NUM_LEDS_PER_STRIP, "B exceeds its channel buffer");
static_assert(NUM_R <= NUM_LEDS_PER_STRIP, "R exceeds its channel buffer");
static_assert(NUM_C <= NUM_LEDS_PER_STRIP, "C exceeds its channel buffer");
static_assert(NUM_3 <= NUM_LEDS_PER_STRIP, "3 exceeds its channel buffer");
static_assert(NUM_P <= NUM_LEDS_PER_STRIP, "P exceeds its channel buffer");
static_assert(NUM_O <= NUM_LEDS_PER_STRIP, "O exceeds its channel buffer");
static_assert(NUM_U <= NUM_LEDS_PER_STRIP,
              "Underline exceeds its channel buffer");
static_assert(NUM_F <= NUM_LEDS_PER_STRIP, "Flag exceeds its channel buffer");

// Channel offsets determined by the control-box and patch-box wiring.
constexpr int THREE_START = 0;
constexpr int C_START = NUM_LEDS_PER_STRIP;
constexpr int R_START = 2 * NUM_LEDS_PER_STRIP;
constexpr int B_START = 3 * NUM_LEDS_PER_STRIP;
constexpr int F_START = 4 * NUM_LEDS_PER_STRIP;
constexpr int U_START = 5 * NUM_LEDS_PER_STRIP;
constexpr int O_START = 6 * NUM_LEDS_PER_STRIP;
constexpr int P_START = 7 * NUM_LEDS_PER_STRIP;

constexpr uint8_t INITIAL_BRIGHTNESS = 140;
constexpr uint8_t MAX_BRIGHTNESS = 240;
constexpr uint16_t IDLE_FRAME_LENGTH_MS = 100;

#include "DancePattern.h"

#include "LedHardware.h"
#include "LetterPatterns.h"

void runDancePattern(int cycles) {
  for (int i = 0; i < NUM_B; i += 2) leds[B_START + i] = CRGB::Black;
  for (int i = 0; i < NUM_R; i += 2) leds[R_START + i] = CRGB::Black;
  for (int i = 0; i < NUM_C; i += 2) leds[C_START + i] = CRGB::Black;
  for (int i = 0; i < NUM_3; i += 2) leds[THREE_START + i] = CRGB::Black;
  for (int i = 0; i < NUM_P; i += 2) leds[P_START + i] = CRGB::Black;
  for (int i = 0; i < NUM_O; i += 2) leds[O_START + i] = CRGB::Black;

  LEDS.show();
  LEDS.delay(1000);

  for (int i = 0; i < cycles; ++i) {
    shiftLetters(1);
    LEDS.show();
    LEDS.delay(300);
    shiftLetters(1);
    LEDS.show();
    LEDS.delay(300);
    shiftLetters(-1);
    LEDS.show();
    LEDS.delay(300);
    shiftLetters(-1);
    LEDS.show();
    LEDS.delay(300);
  }
}


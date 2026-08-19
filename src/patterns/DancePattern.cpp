#include "DancePattern.h"

#include "../core/LedHardware.h"
#include "LetterPatterns.h"
#include "../core/SignTransitions.h"

namespace {
void danceDelay(unsigned long milliseconds, uint8_t timeScalePercent) {
  LEDS.delay((milliseconds * timeScalePercent) / 100UL);
}
}  // namespace

void runDancePattern(int cycles, uint8_t timeScalePercent) {
  captureSignTransitionSource();
  for (int i = 0; i < NUM_B; i += 2) leds[B_START + i] = CRGB::Black;
  for (int i = 0; i < NUM_R; i += 2) leds[R_START + i] = CRGB::Black;
  for (int i = 0; i < NUM_C; i += 2) leds[C_START + i] = CRGB::Black;
  for (int i = 0; i < NUM_3; i += 2) leds[THREE_START + i] = CRGB::Black;
  for (int i = 0; i < NUM_P; i += 2) leds[P_START + i] = CRGB::Black;
  for (int i = 0; i < NUM_O; i += 2) leds[O_START + i] = CRGB::Black;

  fadeIntoCurrentSignFrame(16, 25, timeScalePercent);
  danceDelay(1000, timeScalePercent);

  for (int i = 0; i < cycles; ++i) {
    shiftLetters(1);
    LEDS.show();
    danceDelay(300, timeScalePercent);
    shiftLetters(1);
    LEDS.show();
    danceDelay(300, timeScalePercent);
    shiftLetters(-1);
    LEDS.show();
    danceDelay(300, timeScalePercent);
    shiftLetters(-1);
    LEDS.show();
    danceDelay(300, timeScalePercent);
  }
}

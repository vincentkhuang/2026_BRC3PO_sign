#include "FadeOutPattern.h"

#include "LedHardware.h"
#include "PatternTiming.h"

void runFadeOutPattern() {
  constexpr uint8_t FADE_FRAMES = 48;
  constexpr uint8_t FADE_AMOUNT = 18;

  for (uint8_t frame = 0; frame < FADE_FRAMES; ++frame) {
    for (int pixel = 0; pixel < NUM_LEDS; ++pixel) {
      leds[pixel].fadeToBlackBy(FADE_AMOUNT);
    }
    LEDS.show();
    patternDelay(24);
  }

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  LEDS.show();
  patternDelay(500);
}

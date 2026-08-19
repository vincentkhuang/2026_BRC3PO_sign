#include "PowerUpPattern.h"

#include "PatternColors.h"
#include "PatternTiming.h"
#include "SignLayout.h"

namespace {
void renderLetterFlicker(uint8_t intensity) {
  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    CRGB flicker = signRainbowColor(letter, LETTER_COUNT);
    flicker.nscale8_video(random8(intensity / 2, intensity));
    fillLetter(letter, flicker);
  }
}
}  // namespace

void runPowerUpPattern() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  // Flicker the letters while the underline fills like a progress meter.
  for (int progress = 0; progress < NUM_U; ++progress) {
    uint8_t intensity = map(progress, 0, NUM_U - 1, 35, 190);
    renderLetterFlicker(intensity);
    Strip_U[progress] = signRainbowColor(progress, NUM_U);

    CRGB flagColor = ((progress / 10) % 2 == 0)
                         ? signRainbowColor(progress, NUM_U, 224)
                         : CRGB::Black;
    fill_solid(Strip_F, NUM_F, flagColor);

    LEDS.show();
    patternDelay(18);
  }

  // Lock each letter on in reading order.
  clearAllLetters();
  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    fillLetter(letter, signRainbowColor(letter, LETTER_COUNT));
    LEDS.show();
    patternDelay(180);
  }

  fill_solid(leds, NUM_LEDS, CRGB::White);
  LEDS.show();
  patternDelay(120);

  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    fillLetter(letter, signRainbowColor(letter, LETTER_COUNT));
  }
  fillSignRainbow(Strip_U, NUM_U);
  fill_solid(Strip_F, NUM_F, CRGB::Magenta);
  LEDS.show();
  patternDelay(1000);
}

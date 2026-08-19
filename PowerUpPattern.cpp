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
  fadePatternToBlack();

  // Flicker the letters while the underline fills like a progress meter.
  for (int progress = 0; progress < NUM_U; ++progress) {
    uint8_t intensity = map(progress, 0, NUM_U - 1, 35, 190);
    renderLetterFlicker(intensity);
    // Power-Up is the deliberate left-to-right progress-bar pattern.
    int underlinePosition = progress;
    Strip_U[underlinePosition] =
        signRainbowColor(underlinePosition, NUM_U);

    CRGB flagColor = ((progress / 10) % 2 == 0)
                         ? signRainbowColor(progress, NUM_U, 224)
                         : CRGB::Black;
    fill_solid(Strip_F, NUM_F, flagColor);

    LEDS.show();
    patternDelay(18);
  }

  // Lock each letter on in reading order.
  captureSignTransitionSource();
  clearAllLetters();
  fadeIntoCurrentPatternFrame(12);
  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    fillLetter(letter, signRainbowColor(letter, LETTER_COUNT));
    LEDS.show();
    patternDelay(180);
  }

  // Preserve the locked letter colors as the remaining sign elements settle
  // into their final state; avoid a full-sign white flash between them.
  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    fillLetter(letter, signRainbowColor(letter, LETTER_COUNT));
  }
  fillSignRainbow(Strip_U, NUM_U);
  fill_solid(Strip_F, NUM_F, CRGB::Magenta);
  LEDS.show();
  patternDelay(1000);
}

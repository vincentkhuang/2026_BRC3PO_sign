#include "ProtocolGoldPattern.h"

#include "PatternTiming.h"
#include "../core/SignLayout.h"

namespace {
constexpr uint16_t FRAME_COUNT = 300;
constexpr uint8_t SPARKLES_PER_FRAME = 3;
constexpr uint8_t BACKGROUND_HUE_STEP = 43;
constexpr uint8_t TRANSITION_FRAME_COUNT = 24;
uint8_t nextBackgroundHue = 24;

CRGB scaledColor(CRGB color, uint8_t scale) {
  color.nscale8_video(scale);
  return color;
}

void renderProtocolBackground(CRGB backgroundColor, uint16_t frame) {
  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    fillLetter(letter, backgroundColor);
  }

  uint8_t underlineLevel = 160 + scale8(sin8(frame * 2), 95);
  fill_solid(Strip_U, NUM_U, scaledColor(CRGB::Gold, underlineLevel));

  uint8_t flagLevel = 180 + scale8(sin8(frame * 3), 75);
  fill_solid(Strip_F, NUM_F, scaledColor(backgroundColor, flagLevel));
}

void fadeIntoProtocolBackground(CRGB backgroundColor) {
  captureSignTransitionSource();
  renderProtocolBackground(backgroundColor, 0);
  fadeIntoCurrentPatternFrame(TRANSITION_FRAME_COUNT);
}
}  // namespace

void runProtocolGoldPattern() {
  CRGB backgroundColor = CHSV(nextBackgroundHue, 220, 255);
  nextBackgroundHue += BACKGROUND_HUE_STEP;
  fadeIntoProtocolBackground(backgroundColor);

  for (uint16_t frame = 0; frame < FRAME_COUNT; ++frame) {
    renderProtocolBackground(backgroundColor, frame);

    for (uint8_t sparkle = 0; sparkle < SPARKLES_PER_FRAME; ++sparkle) {
      uint8_t letter = random8(LETTER_COUNT);
      int position = random16(letterLength(letter));
      leds[letterStart(letter) + position] = CRGB::White;
    }

    LEDS.show();
    patternDelay(30);
  }
}

#include "ProtocolGoldPattern.h"

#include "PatternTiming.h"
#include "SignLayout.h"

namespace {
constexpr uint16_t FRAME_COUNT = 300;
constexpr uint8_t SPARKLES_PER_FRAME = 3;

CRGB scaledColor(CRGB color, uint8_t scale) {
  color.nscale8_video(scale);
  return color;
}
}  // namespace

void runProtocolGoldPattern() {
  for (uint16_t frame = 0; frame < FRAME_COUNT; ++frame) {
    CRGB baseGold = scaledColor(CRGB::Gold, 150);
    for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
      fillLetter(letter, baseGold);
    }

    uint8_t underlineLevel = 45 + scale8(sin8(frame * 2), 100);
    fill_solid(Strip_U, NUM_U, scaledColor(CRGB::Gold, underlineLevel));

    uint8_t flagLevel = 70 + scale8(sin8(frame * 3), 150);
    fill_solid(Strip_F, NUM_F, scaledColor(CRGB::Red, flagLevel));

    for (uint8_t sparkle = 0; sparkle < SPARKLES_PER_FRAME; ++sparkle) {
      uint8_t letter = random8(LETTER_COUNT);
      int position = random16(letterLength(letter));
      leds[letterStart(letter) + position] = CRGB::White;
    }

    LEDS.show();
    patternDelay(30);
  }
}

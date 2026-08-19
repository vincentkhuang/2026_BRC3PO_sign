#include "ProtocolGoldPattern.h"

#include "PatternTiming.h"
#include "SignLayout.h"

namespace {
constexpr uint16_t FRAME_COUNT = 300;
constexpr uint8_t SPARKLES_PER_FRAME = 3;
constexpr uint8_t BACKGROUND_HUE_STEP = 43;
constexpr uint8_t TRANSITION_FRAME_COUNT = 24;
uint8_t nextBackgroundHue = 24;
CRGB transitionStart[NUM_LEDS];

CRGB scaledColor(CRGB color, uint8_t scale) {
  color.nscale8_video(scale);
  return color;
}

uint8_t blendChannel(uint8_t start, uint8_t target, uint8_t amount) {
  return static_cast<uint8_t>(
      (static_cast<uint16_t>(start) * (255 - amount) +
       static_cast<uint16_t>(target) * amount + 127) /
      255);
}

CRGB blendColor(CRGB start, CRGB target, uint8_t amount) {
  return CRGB(blendChannel(start.r, target.r, amount),
              blendChannel(start.g, target.g, amount),
              blendChannel(start.b, target.b, amount));
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
  for (int pixel = 0; pixel < NUM_LEDS; ++pixel) {
    transitionStart[pixel] = leds[pixel];
  }

  for (uint8_t frame = 1; frame <= TRANSITION_FRAME_COUNT; ++frame) {
    renderProtocolBackground(backgroundColor, 0);
    uint8_t amount =
        static_cast<uint8_t>((static_cast<uint16_t>(frame) * 255) /
                             TRANSITION_FRAME_COUNT);
    for (int pixel = 0; pixel < NUM_LEDS; ++pixel) {
      leds[pixel] = blendColor(transitionStart[pixel], leds[pixel], amount);
    }
    LEDS.show();
    patternDelay(20);
  }
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

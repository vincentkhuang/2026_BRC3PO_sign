#include "SignTransitions.h"

#include "LedHardware.h"

namespace {
CRGB transitionSource[NUM_LEDS];
CRGB transitionTarget[NUM_LEDS];

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

bool framesMatch() {
  for (int pixel = 0; pixel < NUM_LEDS; ++pixel) {
    if (transitionSource[pixel].r != transitionTarget[pixel].r ||
        transitionSource[pixel].g != transitionTarget[pixel].g ||
        transitionSource[pixel].b != transitionTarget[pixel].b) {
      return false;
    }
  }
  return true;
}
}  // namespace

void captureSignTransitionSource() {
  for (int pixel = 0; pixel < NUM_LEDS; ++pixel) {
    transitionSource[pixel] = leds[pixel];
  }
}

void fadeIntoCurrentSignFrame(uint8_t frameCount,
                              unsigned long frameDelayMilliseconds,
                              uint8_t timeScalePercent) {
  for (int pixel = 0; pixel < NUM_LEDS; ++pixel) {
    transitionTarget[pixel] = leds[pixel];
  }

  if (framesMatch()) return;

  unsigned long fractionalHundredths = 0;
  for (uint16_t frame = 1; frame <= frameCount; ++frame) {
    uint8_t amount = static_cast<uint8_t>(
        (static_cast<uint16_t>(frame) * 255) / frameCount);
    for (int pixel = 0; pixel < NUM_LEDS; ++pixel) {
      leds[pixel] =
          blendColor(transitionSource[pixel], transitionTarget[pixel], amount);
    }
    LEDS.show();

    unsigned long scaledHundredths =
        frameDelayMilliseconds * timeScalePercent + fractionalHundredths;
    LEDS.delay(scaledHundredths / 100UL);
    fractionalHundredths = scaledHundredths % 100UL;
  }
}

void fadeSignToBlack(uint8_t frameCount,
                     unsigned long frameDelayMilliseconds,
                     uint8_t timeScalePercent) {
  captureSignTransitionSource();
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  fadeIntoCurrentSignFrame(frameCount, frameDelayMilliseconds,
                           timeScalePercent);
}

#include "DiscoStrobePattern.h"

#include "LedHardware.h"

namespace {
constexpr uint8_t STROBE_CYCLE_LENGTH = 4;
constexpr uint8_t ZOOM_BPM = 122;

void drawRainbowDashes(uint8_t startPosition, uint16_t lastPosition,
                       uint8_t period, uint8_t width, uint8_t startHue,
                       uint8_t hueDelta, uint8_t saturation, uint8_t value) {
  uint8_t hue = startHue;
  for (uint16_t i = startPosition; i <= lastPosition; i += period) {
    CRGB color = CHSV(hue, saturation, value);
    uint16_t position = i;
    for (uint8_t w = 0; w < width; ++w) {
      leds[U_START + position] = color;
      ++position;
      if (position >= NUM_U) break;
    }
    hue += hueDelta;
  }
}

void renderDashes(uint8_t period, uint8_t width, int8_t motionSpeed,
                  uint8_t strobeRepeats, uint8_t hueDelta) {
  static uint8_t repeatCounter = 0;
  static int8_t startPosition = 0;
  static uint8_t startHue = 0;

  ++startHue;
  ++repeatCounter;
  if (repeatCounter >= strobeRepeats) {
    repeatCounter = 0;
    startPosition += motionSpeed;

    if (startPosition >= period) {
      while (startPosition >= period) startPosition -= period;
      startHue -= hueDelta;
    } else if (startPosition < 0) {
      while (startPosition < 0) startPosition += period;
      startHue += hueDelta;
    }
  }

  drawRainbowDashes(startPosition, NUM_U - 1, period, width, startHue,
                    hueDelta, 208, 255);
}
}  // namespace

void renderDiscoStrobePattern() {
  fill_solid(Strip_U, NUM_U, CRGB::Black);

  static uint8_t strobePhase = 0;
  strobePhase = (strobePhase + 1) % STROBE_CYCLE_LENGTH;
  if (strobePhase != 0) return;

  uint8_t dashPeriod = beatsin8(8, 4, 10);
  uint8_t dashWidth = (dashPeriod / 4) + 1;
  int8_t motionSpeed = beatsin8(ZOOM_BPM / 2, 1, dashPeriod);
  if (motionSpeed >= dashPeriod / 2) {
    motionSpeed = 0 - (dashPeriod - motionSpeed);
  }

  uint8_t cycle = beat8(2);
  uint8_t easedCycle = ease8InOutCubic(ease8InOutCubic(cycle));
  uint8_t hueDelta = scale8(cubicwave8(easedCycle), 130);
  renderDashes(dashPeriod, dashWidth, motionSpeed, 1, hueDelta);
}

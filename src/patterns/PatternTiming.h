#pragma once

#include "../core/LedHardware.h"
#include "../core/SignTransitions.h"

// Run every coordinated-pattern hold and animation frame 20% slower than the
// original preview values. Keeping the scale here makes the whole show easy to
// retune without introducing slightly different hand-rounded delays.
constexpr unsigned long PATTERN_TIME_SCALE_PERCENT = 120;
constexpr uint8_t PATTERN_TRANSITION_FRAME_COUNT = 24;
constexpr unsigned long PATTERN_TRANSITION_FRAME_MS = 20;

inline void patternDelay(unsigned long milliseconds) {
  // Carry fractional milliseconds forward so repeated short frame delays stay
  // exactly 20% slower in aggregate instead of accumulating rounding error.
  static uint8_t fractionalHundredths = 0;
  unsigned long scaledHundredths =
      milliseconds * PATTERN_TIME_SCALE_PERCENT + fractionalHundredths;
  LEDS.delay(scaledHundredths / 100);
  fractionalHundredths = scaledHundredths % 100;
}

inline void fadeIntoCurrentPatternFrame(
    uint8_t frameCount = PATTERN_TRANSITION_FRAME_COUNT) {
  fadeIntoCurrentSignFrame(frameCount, PATTERN_TRANSITION_FRAME_MS,
                           PATTERN_TIME_SCALE_PERCENT);
}

inline void fadePatternToBlack(
    uint8_t frameCount = PATTERN_TRANSITION_FRAME_COUNT) {
  fadeSignToBlack(frameCount, PATTERN_TRANSITION_FRAME_MS,
                  PATTERN_TIME_SCALE_PERCENT);
}

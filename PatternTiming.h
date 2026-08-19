#pragma once

#include "LedHardware.h"

// Run every coordinated-pattern hold and animation frame 20% slower than the
// original preview values. Keeping the scale here makes the whole show easy to
// retune without introducing slightly different hand-rounded delays.
constexpr unsigned long PATTERN_TIME_SCALE_PERCENT = 120;

inline void patternDelay(unsigned long milliseconds) {
  // Carry fractional milliseconds forward so repeated short frame delays stay
  // exactly 10% slower in aggregate instead of accumulating rounding error.
  static uint8_t fractionalHundredths = 0;
  unsigned long scaledHundredths =
      milliseconds * PATTERN_TIME_SCALE_PERCENT + fractionalHundredths;
  LEDS.delay(scaledHundredths / 100);
  fractionalHundredths = scaledHundredths % 100;
}

#pragma once

#include <stdint.h>

// Capture the sign's current pixel buffer before drawing a new target frame.
void captureSignTransitionSource();

// Blend from the captured frame into the pixels currently stored in `leds`.
// The delay is scaled so coordinated and legacy shows can keep their own pace.
void fadeIntoCurrentSignFrame(uint8_t frameCount,
                              unsigned long frameDelayMilliseconds,
                              uint8_t timeScalePercent = 100);

// Fade the complete installation from its current state to black.
void fadeSignToBlack(uint8_t frameCount,
                     unsigned long frameDelayMilliseconds,
                     uint8_t timeScalePercent = 100);

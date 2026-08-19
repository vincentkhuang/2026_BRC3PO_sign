#pragma once

#include "../core/LedHardware.h"

inline CRGB signRainbowColor(int index, int count, uint8_t hueOffset = 0,
                             uint8_t saturation = 255,
                             uint8_t value = 255) {
  if (count <= 0) return CRGB::Black;
  uint8_t hue = hueOffset +
                static_cast<uint8_t>((static_cast<uint32_t>(index) * 256UL) /
                                     static_cast<uint32_t>(count));
  return CHSV(hue, saturation, value);
}

inline void fillSignRainbow(CRGBSet& strip, int count, uint8_t hueOffset = 0,
                            uint8_t saturation = 255,
                            uint8_t value = 255) {
  for (int pixel = 0; pixel < count; ++pixel) {
    strip[pixel] = signRainbowColor(pixel, count, hueOffset, saturation, value);
  }
}

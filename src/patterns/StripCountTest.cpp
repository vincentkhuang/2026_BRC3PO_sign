#include "StripCountTest.h"

#include "../core/LedHardware.h"

namespace {

constexpr int COUNT_BLOCK_LENGTH = 10;

static_assert(NUM_STRIPS == 8, "The strip-count test expects eight OctoWS2811 outputs");
static_assert(NUM_LEDS_PER_STRIP == 256,
              "The strip-count test expects 256 pixels per output");

bool isLitBlock(int pixel) {
  return ((pixel / COUNT_BLOCK_LENGTH) % 2) == 0;
}

void fillCountedStrip(int start, const CRGB& color) {
  for (int pixel = 0; pixel < NUM_LEDS_PER_STRIP; ++pixel) {
    leds[start + pixel] = isLitBlock(pixel) ? color : CRGB::Black;
  }
}

void fillCountedRainbowStrip(int start) {
  for (int pixel = 0; pixel < NUM_LEDS_PER_STRIP; ++pixel) {
    if (!isLitBlock(pixel)) {
      leds[start + pixel] = CRGB::Black;
      continue;
    }

    const uint8_t hue = static_cast<uint8_t>(
        (static_cast<unsigned long>(pixel) * 255UL) /
        (NUM_LEDS_PER_STRIP - 1));
    leds[start + pixel] = CHSV(hue, 255, 255);
  }
}

}  // namespace

void renderStripCountTest() {
  fill_solid(leds, NUM_LEDS, CRGB::Black);

  fillCountedStrip(B_START, CRGB::Red);
  fillCountedStrip(R_START, CRGB::Orange);
  fillCountedStrip(C_START, CRGB::Yellow);
  fillCountedStrip(THREE_START, CRGB::Green);
  fillCountedStrip(P_START, CRGB::Blue);
  fillCountedStrip(O_START, CRGB::Purple);
  fillCountedStrip(U_START, CRGB::White);
  fillCountedRainbowStrip(F_START);

  LEDS.show();
}

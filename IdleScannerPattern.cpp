#include "IdleScannerPattern.h"

#include "Config.h"
#include "Controls.h"
#include "LedHardware.h"
#include "LetterPatterns.h"

namespace {
unsigned long previousFrameTime = 0;
constexpr unsigned long SCANNER_DELAY_MS = 20;
int position = 0;
int direction = 1;
constexpr int EYE_PADDING = 10;
const CRGB EYE_COLOR = CRGB::White;
const CRGB FLAG_COLOR = CRGB::Red;
}  // namespace

void initIdleScannerPattern() {
  previousFrameTime = millis();
}

void renderIdleScannerPattern() {
  readControls();
  setLetterColor(currentColor);
  fill_solid(Strip_F, NUM_F, FLAG_COLOR);
  LEDS.show();

  unsigned long now = millis();
  if (now - previousFrameTime >= SCANNER_DELAY_MS) {
    if (position >= 2 && position <= NUM_U - 3) {
      fill_solid(Strip_U, NUM_U, CRGB::Black);
    }

    leds[U_START + position] = EYE_COLOR;
    for (int i = 0; i < EYE_PADDING; ++i) {
      if (position - i >= 0) leds[U_START + position - i] = EYE_COLOR;
      if (position + i < NUM_U - 1) leds[U_START + position + i] = EYE_COLOR;

      if (position + i + 1 <= NUM_U - 1) {
        leds[U_START + position + i + 1].nscale8(200);
      }
      if (position - i - 1 >= 0) {
        leds[U_START + position - i - 1].nscale8(200);
      }
    }

    position += direction;
    if (position < 0) {
      position = 1;
      direction = -direction;
    } else if (position >= NUM_U - 1) {
      position = NUM_U - 2;
      direction = -direction;
    }
  }

  previousFrameTime = now;
  LEDS.show();
  LEDS.delay(IDLE_FRAME_LENGTH_MS);
}


#include "Controls.h"

#include "Config.h"

uint8_t letterHue = 0;
uint8_t currentBrightness = INITIAL_BRIGHTNESS;
CRGB currentColor = CRGB::Black;

void readControls() {
  if (ENABLE_COLOR_KNOB) {
    letterHue = map(analogRead(POT_COLOR), 0, 1023, 0, 255);
  } else {
    letterHue = FIXED_LETTER_HUE;
  }

  if (ENABLE_BRIGHTNESS_KNOB) {
    currentBrightness = map(
        analogRead(POT_BRIGHTNESS), 0, 1023, 0, MAX_BRIGHTNESS);
  } else {
    currentBrightness = FIXED_BRIGHTNESS;
  }

  CHSV selectedColor(letterHue, 255, currentBrightness);
  currentColor = selectedColor;
  LEDS.setBrightness(currentBrightness);
}

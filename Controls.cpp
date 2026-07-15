#include "Controls.h"

#include "Config.h"

uint8_t letterHue = 0;
uint8_t currentBrightness = INITIAL_BRIGHTNESS;
CRGB currentColor = CRGB::Black;

void readControls() {
  letterHue = map(analogRead(POT_COLOR), 0, 1023, 0, 255);
  currentBrightness = map(
      analogRead(POT_BRIGHTNESS), 0, 1023, 0, MAX_BRIGHTNESS);

  CHSV selectedColor(letterHue, 255, currentBrightness);
  currentColor = selectedColor;
  LEDS.setBrightness(currentBrightness);
}

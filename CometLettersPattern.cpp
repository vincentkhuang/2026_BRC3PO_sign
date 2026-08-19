#include "CometLettersPattern.h"

#include "PatternColors.h"
#include "PatternTiming.h"
#include "SignLayout.h"

namespace {
void drawComet(uint8_t letter, int head, int tailLength, CRGB color) {
  int start = letterStart(letter);
  int length = letterLength(letter);

  for (int i = 0; i < length; ++i) leds[start + i].fadeToBlackBy(70);

  for (int tail = 0; tail < tailLength; ++tail) {
    int position = (head - tail + length) % length;
    CRGB tailColor = color;
    tailColor.nscale8_video(255 - ((tail * 220) / tailLength));
    leds[start + position] += tailColor;
  }
  leds[start + (head % length)] = CRGB::White;
}
}  // namespace

void runCometLettersPattern() {
  clearAllLetters();
  fill_solid(Strip_U, NUM_U, CRGB::Black);
  fill_solid(Strip_F, NUM_F, CRGB::Black);

  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    CRGB color = signRainbowColor(letter, LETTER_COUNT);
    int length = letterLength(letter);
    int tailLength = length / 10;
    if (tailLength < 8) tailLength = 8;

    fill_solid(Strip_U, NUM_U, CRGB::Black);
    CRGB underlineColor = color;
    underlineColor.nscale8_video(110);
    fillUnderlineSegment(letter, underlineColor);
    fill_solid(Strip_F, NUM_F, color);

    for (int head = 0; head < length + tailLength; ++head) {
      drawComet(letter, head % length, tailLength, color);
      LEDS.show();
      patternDelay(16);
    }

    fillLetter(letter, color);
    LEDS.show();
    patternDelay(180);
    fillLetter(letter, CRGB::Black);
  }

  fillSignRainbow(Strip_F, NUM_F, 224);
}

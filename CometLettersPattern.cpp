#include "CometLettersPattern.h"

#include "PatternColors.h"
#include "PatternTiming.h"
#include "SignLayout.h"

namespace {
void fillLetterRainbow(uint8_t letter, uint8_t value = 255) {
  int start = letterStart(letter);
  int length = letterLength(letter);
  for (int pixel = 0; pixel < length; ++pixel) {
    leds[start + pixel] = signRainbowColor(pixel, length, 0, 255, value);
  }
}

void fillUnderlineRainbowSegment(uint8_t letter, uint8_t value) {
  int start = underlineSegmentStart(letter);
  int end = underlineSegmentEnd(letter);
  int length = end - start;
  for (int pixel = 0; pixel < length; ++pixel) {
    Strip_U[start + pixel] =
        signRainbowColor(pixel, length, 0, 255, value);
  }
}

void drawComet(uint8_t letter, int head, int tailLength) {
  int start = letterStart(letter);
  int length = letterLength(letter);

  for (int i = 0; i < length; ++i) leds[start + i].fadeToBlackBy(70);

  for (int tail = 0; tail < tailLength; ++tail) {
    int position = (head - tail + length) % length;
    CRGB tailColor = signRainbowColor(position, length);
    tailColor.nscale8_video(255 - ((tail * 220) / tailLength));
    leds[start + position] += tailColor;
  }
  leds[start + (head % length)] = CRGB::White;
}
}  // namespace

void runCometLettersPattern() {
  clearAllLetters();
  fill_solid(Strip_U, NUM_U, CRGB::Black);
  fillSignRainbow(Strip_F, NUM_F, 224);

  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    int length = letterLength(letter);
    int tailLength = length / 10;
    if (tailLength < 8) tailLength = 8;

    fill_solid(Strip_U, NUM_U, CRGB::Black);
    fillUnderlineRainbowSegment(letter, 110);

    for (int head = 0; head < length + tailLength; ++head) {
      drawComet(letter, head % length, tailLength);
      LEDS.show();
      patternDelay(16);
    }

    fillLetterRainbow(letter);
    LEDS.show();
    patternDelay(180);
    fillLetter(letter, CRGB::Black);
  }
}

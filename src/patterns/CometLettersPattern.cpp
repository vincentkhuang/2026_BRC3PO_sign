#include "CometLettersPattern.h"

#include "PatternColors.h"
#include "PatternTiming.h"
#include "../core/SignLayout.h"

namespace {
uint8_t nextAccentHue = 0;
constexpr uint8_t ACCENT_HUE_STEP = 43;

void fillLetterRainbow(uint8_t letter, uint8_t value = 255) {
  int start = letterStart(letter);
  int length = letterLength(letter);
  for (int pixel = 0; pixel < length; ++pixel) {
    leds[start + pixel] = signRainbowColor(pixel, length, 0, 255, value);
  }
}

void drawComet(uint8_t letter, int head, int tailLength) {
  int start = letterStart(letter);
  int length = letterLength(letter);

  for (int i = 0; i < length; ++i) leds[start + i].fadeToBlackBy(70);

  for (int tail = 0; tail < tailLength; ++tail) {
    int position = head - tail;
    if (position < 0 || position >= length) continue;
    CRGB tailColor = signRainbowColor(position, length);
    tailColor.nscale8_video(255 - ((tail * 220) / tailLength));
    leds[start + position] += tailColor;
  }
  if (head < length) leds[start + head] = CRGB::White;
}

void drawAccumulatingComet(uint8_t letter, int head) {
  int start = letterStart(letter);
  int length = letterLength(letter);

  if (head > 0) {
    int finishedPixel = head - 1;
    leds[start + finishedPixel] =
        signRainbowColor(finishedPixel, length);
  }
  leds[start + head] = CRGB::White;
}
}  // namespace

void runCometLettersPattern() {
  CRGB accentColor = CHSV(nextAccentHue, 235, 255);
  nextAccentHue += ACCENT_HUE_STEP;

  captureSignTransitionSource();
  clearAllLetters();
  fill_solid(Strip_U, NUM_U, accentColor);
  fill_solid(Strip_F, NUM_F, accentColor);
  fadeIntoCurrentPatternFrame();

  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    int length = letterLength(letter);
    int tailLength = length / 10;
    if (tailLength < 8) tailLength = 8;

    for (int head = 0; head < length + tailLength; ++head) {
      drawComet(letter, head, tailLength);
      LEDS.show();
      patternDelay(16);
    }

    // Give the accumulating pass a completely clean, dark letter to paint.
    fillLetter(letter, CRGB::Black);
    LEDS.show();
    patternDelay(100);

    // The second comet paints a permanent rainbow behind its white head,
    // turning the moving trail into the fully illuminated letter gradually.
    for (int head = 0; head < length; ++head) {
      drawAccumulatingComet(letter, head);
      LEDS.show();
      patternDelay(16);
    }

    fillLetterRainbow(letter);
    LEDS.show();
    patternDelay(180);
  }
}

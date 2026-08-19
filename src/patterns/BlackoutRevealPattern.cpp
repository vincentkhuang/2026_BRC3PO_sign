#include "BlackoutRevealPattern.h"

#include "PatternColors.h"
#include "PatternTiming.h"
#include "../core/SignLayout.h"

namespace {
constexpr int LONGEST_LETTER = NUM_B;

void setSynchronizedLetterPixel(int progress, bool reveal) {
  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    int length = letterLength(letter);
    int position = (progress * length) / LONGEST_LETTER;
    if (position < length) {
      leds[letterStart(letter) + position] =
          reveal ? signRainbowColor(letter, LETTER_COUNT) : CRGB::Black;
    }
  }
}

void setSynchronizedFlagPixels(int progress, bool reveal) {
  // The flag is longer than the 141-step letter sweep, so some animation
  // steps must advance two flag pixels. Covering the complete normalized
  // range prevents skipped pixels from lingering during blackout or popping
  // on only when the reveal finishes.
  int start = (progress * NUM_F) / LONGEST_LETTER;
  int end = ((progress + 1) * NUM_F) / LONGEST_LETTER;
  for (int position = start; position < end; ++position) {
    Strip_F[position] =
        reveal ? signRainbowColor(position, NUM_F, 224) : CRGB::Black;
  }
}

void fillRainbowSign() {
  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    fillLetter(letter, signRainbowColor(letter, LETTER_COUNT));
  }
  fillSignRainbow(Strip_U, NUM_U);
  fillSignRainbow(Strip_F, NUM_F, 224);
}
}  // namespace

void runBlackoutRevealPattern() {
  captureSignTransitionSource();
  fillRainbowSign();
  fadeIntoCurrentPatternFrame();
  patternDelay(600);

  // Sweep darkness clockwise around every letter at the same normalized rate.
  for (int progress = 0; progress < LONGEST_LETTER; ++progress) {
    setSynchronizedLetterPixel(progress, false);
    int underlinePosition = (progress * NUM_U) / LONGEST_LETTER;
    if (underlinePosition < NUM_U) Strip_U[underlinePosition] = CRGB::Black;
    setSynchronizedFlagPixels(progress, false);
    LEDS.show();
    patternDelay(12);
  }

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  LEDS.show();
  patternDelay(500);

  // Reveal the sign using the same installed clockwise paths.
  for (int progress = 0; progress < LONGEST_LETTER; ++progress) {
    setSynchronizedLetterPixel(progress, true);
    int underlinePosition = (progress * NUM_U) / LONGEST_LETTER;
    if (underlinePosition < NUM_U) {
      int reversePosition = NUM_U - 1 - underlinePosition;
      Strip_U[reversePosition] =
          signRainbowColor(reversePosition, NUM_U);
    }
    setSynchronizedFlagPixels(progress, true);
    LEDS.show();
    patternDelay(12);
  }

  fillRainbowSign();
  LEDS.show();
  patternDelay(1000);
}

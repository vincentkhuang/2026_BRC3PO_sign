#include "BlackoutRevealPattern.h"

#include "PatternColors.h"
#include "PatternTiming.h"
#include "SignLayout.h"

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

void fillRainbowSign() {
  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    fillLetter(letter, signRainbowColor(letter, LETTER_COUNT));
  }
  fillSignRainbow(Strip_U, NUM_U);
  fillSignRainbow(Strip_F, NUM_F, 224);
}
}  // namespace

void runBlackoutRevealPattern() {
  fillRainbowSign();
  LEDS.show();
  patternDelay(600);

  // Sweep darkness clockwise around every letter at the same normalized rate.
  for (int progress = 0; progress < LONGEST_LETTER; ++progress) {
    setSynchronizedLetterPixel(progress, false);
    int underlinePosition = (progress * NUM_U) / LONGEST_LETTER;
    if (underlinePosition < NUM_U) Strip_U[underlinePosition] = CRGB::Black;
    int flagPosition = (progress * NUM_F) / LONGEST_LETTER;
    if (flagPosition < NUM_F) Strip_F[flagPosition] = CRGB::Black;
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
    int flagPosition = (progress * NUM_F) / LONGEST_LETTER;
    if (flagPosition < NUM_F) {
      Strip_F[flagPosition] = signRainbowColor(flagPosition, NUM_F, 224);
    }
    LEDS.show();
    patternDelay(12);
  }

  fillRainbowSign();
  LEDS.show();
  patternDelay(1000);
}

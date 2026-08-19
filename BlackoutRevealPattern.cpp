#include "BlackoutRevealPattern.h"

#include "SignLayout.h"

namespace {
constexpr int LONGEST_LETTER = NUM_B;

void setSynchronizedLetterPixel(int progress, CRGB color) {
  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    int length = letterLength(letter);
    int position = (progress * length) / LONGEST_LETTER;
    if (position < length) leds[letterStart(letter) + position] = color;
  }
}
}  // namespace

void runBlackoutRevealPattern() {
  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    fillLetter(letter, CRGB::Gold);
  }
  fill_solid(Strip_U, NUM_U, CRGB::Gold);
  fill_solid(Strip_F, NUM_F, CRGB::Red);
  LEDS.show();
  LEDS.delay(600);

  // Sweep darkness clockwise around every letter at the same normalized rate.
  for (int progress = 0; progress < LONGEST_LETTER; ++progress) {
    setSynchronizedLetterPixel(progress, CRGB::Black);
    int underlinePosition = (progress * NUM_U) / LONGEST_LETTER;
    if (underlinePosition < NUM_U) Strip_U[underlinePosition] = CRGB::Black;
    int flagPosition = (progress * NUM_F) / LONGEST_LETTER;
    if (flagPosition < NUM_F) Strip_F[flagPosition] = CRGB::Black;
    LEDS.show();
    LEDS.delay(12);
  }

  fill_solid(leds, NUM_LEDS, CRGB::Black);
  LEDS.show();
  LEDS.delay(500);

  // Reveal the sign using the same installed clockwise paths.
  for (int progress = 0; progress < LONGEST_LETTER; ++progress) {
    setSynchronizedLetterPixel(progress, CRGB::Gold);
    int underlinePosition = (progress * NUM_U) / LONGEST_LETTER;
    if (underlinePosition < NUM_U) Strip_U[underlinePosition] = CRGB::Gold;
    int flagPosition = (progress * NUM_F) / LONGEST_LETTER;
    if (flagPosition < NUM_F) Strip_F[flagPosition] = CRGB::Red;
    LEDS.show();
    LEDS.delay(12);
  }

  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    fillLetter(letter, CRGB::Gold);
  }
  fill_solid(Strip_U, NUM_U, CRGB::Gold);
  fill_solid(Strip_F, NUM_F, CRGB::Red);
  LEDS.show();
  LEDS.delay(1000);
}

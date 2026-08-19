#include "LetterDominoPattern.h"

#include "SignLayout.h"

void runLetterDominoPattern() {
  constexpr uint8_t CYCLES = 3;

  for (uint8_t cycle = 0; cycle < CYCLES; ++cycle) {
    clearAllLetters();
    fill_solid(Strip_U, NUM_U, CRGB::Black);
    fill_solid(Strip_F, NUM_F, CRGB::Red);

    for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
      fillLetter(letter, CRGB::Gold);
      fillUnderlineSegment(letter, CRGB::White);
      LEDS.show();
      LEDS.delay(170);
      fillUnderlineSegment(letter, CRGB::Gold);
    }

    fill_solid(Strip_F, NUM_F, CRGB::White);
    LEDS.show();
    LEDS.delay(220);
    fill_solid(Strip_F, NUM_F, CRGB::Red);

    for (int letter = LETTER_COUNT - 1; letter >= 0; --letter) {
      fillLetter(letter, CRGB::Black);
      fillUnderlineSegment(letter, CRGB::Black);
      LEDS.show();
      LEDS.delay(130);
    }
  }
}


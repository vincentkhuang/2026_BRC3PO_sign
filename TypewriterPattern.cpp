#include "TypewriterPattern.h"

#include "SignLayout.h"

void runTypewriterPattern() {
  clearAllLetters();
  fill_solid(Strip_U, NUM_U, CRGB::Black);
  fill_solid(Strip_F, NUM_F, CRGB::DarkRed);

  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    fill_solid(Strip_U, NUM_U, CRGB::Black);
    fillUnderlineSegment(letter, CRGB::White);
    LEDS.show();
    LEDS.delay(120);

    fillLetter(letter, CRGB::Gold);
    fillUnderlineSegment(letter, CRGB::Gold);
    LEDS.show();
    LEDS.delay(260);
  }

  fill_solid(Strip_U, NUM_U, CRGB::Gold);
  fill_solid(Strip_F, NUM_F, CRGB::Red);
  LEDS.show();
  LEDS.delay(1400);
}

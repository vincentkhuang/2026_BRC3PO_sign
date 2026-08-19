#include "TypewriterPattern.h"

#include "PatternColors.h"
#include "PatternTiming.h"
#include "SignLayout.h"

void runTypewriterPattern() {
  clearAllLetters();
  fill_solid(Strip_U, NUM_U, CRGB::Black);
  fill_solid(Strip_F, NUM_F, CRGB::Purple);

  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    CRGB letterColor = signRainbowColor(letter, LETTER_COUNT, 128);
    fill_solid(Strip_U, NUM_U, CRGB::Black);
    fillUnderlineSegment(letter, CRGB::White);
    LEDS.show();
    patternDelay(120);

    fillLetter(letter, letterColor);
    fillUnderlineSegment(letter, letterColor);
    LEDS.show();
    patternDelay(260);
  }

  fillSignRainbow(Strip_U, NUM_U, 128);
  fill_solid(Strip_F, NUM_F, CRGB::Magenta);
  LEDS.show();
  patternDelay(1400);
}

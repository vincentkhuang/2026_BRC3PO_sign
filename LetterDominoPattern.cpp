#include "LetterDominoPattern.h"

#include "PatternColors.h"
#include "PatternTiming.h"
#include "SignLayout.h"

void runLetterDominoPattern() {
  constexpr uint8_t CYCLES = 3;

  for (uint8_t cycle = 0; cycle < CYCLES; ++cycle) {
    uint8_t hueOffset = cycle * 43;
    if (cycle == 0) captureSignTransitionSource();
    clearAllLetters();
    fill_solid(Strip_U, NUM_U, CRGB::Black);
    fill_solid(Strip_F, NUM_F,
               signRainbowColor(cycle, CYCLES, hueOffset));
    if (cycle == 0) fadeIntoCurrentPatternFrame();

    for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
      CRGB letterColor = signRainbowColor(letter, LETTER_COUNT, hueOffset);
      fillLetter(letter, letterColor);
      fillUnderlineSegment(letter, CRGB::White);
      LEDS.show();
      patternDelay(170);
      fillUnderlineSegment(letter, letterColor);
    }

    fill_solid(Strip_F, NUM_F, CRGB::White);
    LEDS.show();
    patternDelay(220);
    fill_solid(Strip_F, NUM_F,
               signRainbowColor(cycle + 1, CYCLES, hueOffset));

    for (int letter = LETTER_COUNT - 1; letter >= 0; --letter) {
      fillLetter(letter, CRGB::Black);
      fillUnderlineSegment(letter, CRGB::Black);
      LEDS.show();
      patternDelay(130);
    }
  }
}

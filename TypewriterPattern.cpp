#include "TypewriterPattern.h"

#include "PatternTiming.h"
#include "SignLayout.h"

namespace {
uint8_t nextTypewriterHue = 128;
constexpr uint8_t TYPEWRITER_HUE_STEP = 43;
}  // namespace

void runTypewriterPattern() {
  CRGB runColor = CHSV(nextTypewriterHue, 235, 255);
  nextTypewriterHue += TYPEWRITER_HUE_STEP;

  captureSignTransitionSource();
  clearAllLetters();
  fill_solid(Strip_U, NUM_U, CRGB::Black);
  // Keep the mailbox flag visible for the entire typing sequence.
  fill_solid(Strip_F, NUM_F, runColor);
  fadeIntoCurrentPatternFrame();

  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    fill_solid(Strip_U, NUM_U, CRGB::Black);
    fillUnderlineSegment(letter, CRGB::White);
    LEDS.show();
    patternDelay(120);

    fillLetter(letter, runColor);
    fillUnderlineSegment(letter, runColor);
    LEDS.show();
    patternDelay(260);
  }

  fill_solid(Strip_U, NUM_U, CRGB::Black);
  for (int position = NUM_U - 1; position >= 0; --position) {
    Strip_U[position] = runColor;
    LEDS.show();
    patternDelay(8);
  }

  fill_solid(Strip_F, NUM_F, runColor);
  LEDS.show();
  patternDelay(1400);
}

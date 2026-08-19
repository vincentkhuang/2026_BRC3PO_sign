#include "SignalRelayPattern.h"

#include "PatternColors.h"
#include "PatternTiming.h"
#include "../core/SignLayout.h"

namespace {
void fadeUnderline(uint8_t amount) {
  for (int i = 0; i < NUM_U; ++i) Strip_U[i].fadeToBlackBy(amount);
}
}  // namespace

void runSignalRelayPattern() {
  fadePatternToBlack();

  // The mailbox flag charges from its installed pixel zero toward its end.
  for (int position = 0; position < NUM_F; position += 2) {
    CRGB signalColor = signRainbowColor(position, NUM_F, 224);
    Strip_F[position] = signalColor;
    if (position + 1 < NUM_F) Strip_F[position + 1] = signalColor;
    LEDS.show();
    patternDelay(8);
  }

  uint8_t nextLetter = 0;
  for (int position = 0; position < NUM_U; ++position) {
    fadeUnderline(36);
    Strip_U[position] = CRGB::White;
    if (position > 0) Strip_U[position - 1] = CRGB::Cyan;

    while (nextLetter < LETTER_COUNT &&
           position >= underlineSegmentStart(nextLetter)) {
      fillLetter(nextLetter,
                 signRainbowColor(nextLetter, LETTER_COUNT, 24));
      ++nextLetter;
    }

    LEDS.show();
    patternDelay(12);
  }

  fill_solid(Strip_U, NUM_U, CRGB::Black);
  for (int position = NUM_U - 1; position >= 0; --position) {
    Strip_U[position] = signRainbowColor(position, NUM_U, 24);
    LEDS.show();
    patternDelay(8);
  }

  patternDelay(1200);
}

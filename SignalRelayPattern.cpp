#include "SignalRelayPattern.h"

#include "SignLayout.h"

namespace {
void fadeUnderline(uint8_t amount) {
  for (int i = 0; i < NUM_U; ++i) Strip_U[i].fadeToBlackBy(amount);
}
}  // namespace

void runSignalRelayPattern() {
  clearAllLetters();
  fill_solid(Strip_U, NUM_U, CRGB::Black);
  fill_solid(Strip_F, NUM_F, CRGB::Black);

  // The mailbox flag charges from its installed pixel zero toward its end.
  for (int position = 0; position < NUM_F; position += 2) {
    Strip_F[position] = CRGB::Red;
    if (position + 1 < NUM_F) Strip_F[position + 1] = CRGB::Red;
    LEDS.show();
    LEDS.delay(8);
  }

  uint8_t nextLetter = 0;
  for (int position = 0; position < NUM_U; ++position) {
    fadeUnderline(36);
    Strip_U[position] = CRGB::White;
    if (position > 0) Strip_U[position - 1] = CRGB::Cyan;

    while (nextLetter < LETTER_COUNT &&
           position >= underlineSegmentStart(nextLetter)) {
      fillLetter(nextLetter, CRGB::Gold);
      ++nextLetter;
    }

    LEDS.show();
    LEDS.delay(12);
  }

  fill_solid(Strip_U, NUM_U, CRGB::Gold);
  LEDS.show();
  LEDS.delay(1200);
}

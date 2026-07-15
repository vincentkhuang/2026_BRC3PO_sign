#include "PalettePattern.h"

#include "LedHardware.h"
#include "LetterPatterns.h"

void renderPalettePattern(uint8_t colorIndex) {
  currentPalette = RainbowColors_p;
  uint8_t stripColorIndex = colorIndex;
  for (int i = 0; i < NUM_U; ++i) {
    Strip_U[i] = ColorFromPalette(
        currentPalette, stripColorIndex, 255, currentBlending);
    stripColorIndex += 3;
  }
  for (int i = 0; i < NUM_F; ++i) {
    Strip_F[i] = ColorFromPalette(
        currentPalette, stripColorIndex, 255, currentBlending);
    stripColorIndex += 3;
  }

  currentPalette = PartyColors_p;
  fillLettersFromPalette(colorIndex);
}

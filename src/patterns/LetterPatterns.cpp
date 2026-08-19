#include "LetterPatterns.h"

void setLetterColor(CRGB color) {
  fill_solid(Strip_B, NUM_B, color);
  fill_solid(Strip_R, NUM_R, color);
  fill_solid(Strip_C, NUM_C, color);
  fill_solid(Strip_3, NUM_3, color);
  fill_solid(Strip_P, NUM_P, color);
  fill_solid(Strip_O, NUM_O, color);
}

void fillLettersFromPalette(uint8_t index) {
  fill_solid(Strip_B, NUM_B,
             ColorFromPalette(currentPalette, index, 255, currentBlending));
  fill_solid(Strip_R, NUM_R,
             ColorFromPalette(currentPalette, index + 6, 255, currentBlending));
  fill_solid(Strip_C, NUM_C,
             ColorFromPalette(currentPalette, index + 12, 255, currentBlending));
  fill_solid(Strip_3, NUM_3,
             ColorFromPalette(currentPalette, index + 18, 255, currentBlending));
  fill_solid(Strip_P, NUM_P,
             ColorFromPalette(currentPalette, index + 24, 255, currentBlending));
  fill_solid(Strip_O, NUM_O,
             ColorFromPalette(currentPalette, index + 30, 255, currentBlending));
}

static void shiftLeds(int start, int count, int shift) {
  // This preserves the legacy shift behavior. Endpoint rotation can be fixed
  // independently now that it is isolated here.
  if (shift > 0) {
    leds[start + count] = leds[start];
    for (int j = start + count - 1; j > start; --j) {
      leds[j] = leds[j - shift];
    }
  } else {
    leds[start] = leds[start + count];
    for (int j = start + 1; j < start + count; ++j) {
      leds[j] = leds[j - shift];
    }
  }
}

void shiftLetters(int shift) {
  shiftLeds(B_START, NUM_B, shift);
  shiftLeds(R_START, NUM_R, shift);
  shiftLeds(C_START, NUM_C, shift);
  shiftLeds(THREE_START, NUM_3, shift);
  shiftLeds(P_START, NUM_P, shift);
  shiftLeds(O_START, NUM_O, shift);
}

void letterColorShift(int paletteIndex) {
  currentPalette = RainbowColors_p;

  fill_solid(Strip_B, NUM_B, currentPalette[paletteIndex]);
  fill_solid(Strip_R, NUM_R, currentPalette[(paletteIndex + 1) % 15]);
  fill_solid(Strip_C, NUM_C, currentPalette[(paletteIndex + 2) % 15]);
  fill_solid(Strip_3, NUM_3, currentPalette[(paletteIndex + 3) % 15]);
  fill_solid(Strip_P, NUM_P, currentPalette[(paletteIndex + 4) % 15]);
  fill_solid(Strip_O, NUM_O, currentPalette[(paletteIndex + 5) % 15]);
}


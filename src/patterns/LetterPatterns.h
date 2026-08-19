#pragma once

#include "../core/LedHardware.h"

void setLetterColor(CRGB color);
void fillLettersFromPalette(uint8_t index);
void shiftLetters(int shift);
void letterColorShift(int paletteIndex);

#pragma once

#include <Arduino.h>

#include "LedHardware.h"

constexpr uint8_t LETTER_COUNT = 6;

int letterStart(uint8_t letterIndex);
int letterLength(uint8_t letterIndex);
void fillLetter(uint8_t letterIndex, CRGB color);
void fadeLetter(uint8_t letterIndex, uint8_t amount);
void clearAllLetters();

int underlineSegmentStart(uint8_t letterIndex);
int underlineSegmentEnd(uint8_t letterIndex);
void fillUnderlineSegment(uint8_t letterIndex, CRGB color);

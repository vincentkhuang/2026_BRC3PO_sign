#include "SignLayout.h"

namespace {
constexpr int LETTER_STARTS[LETTER_COUNT] = {
    B_START, R_START, C_START, THREE_START, P_START, O_START};
constexpr int LETTER_LENGTHS[LETTER_COUNT] = {
    NUM_B, NUM_R, NUM_C, NUM_3, NUM_P, NUM_O};
}  // namespace

int letterStart(uint8_t letterIndex) {
  return letterIndex < LETTER_COUNT ? LETTER_STARTS[letterIndex] : 0;
}

int letterLength(uint8_t letterIndex) {
  return letterIndex < LETTER_COUNT ? LETTER_LENGTHS[letterIndex] : 0;
}

void fillLetter(uint8_t letterIndex, CRGB color) {
  int start = letterStart(letterIndex);
  int length = letterLength(letterIndex);
  for (int i = 0; i < length; ++i) leds[start + i] = color;
}

void fadeLetter(uint8_t letterIndex, uint8_t amount) {
  int start = letterStart(letterIndex);
  int length = letterLength(letterIndex);
  for (int i = 0; i < length; ++i) leds[start + i].fadeToBlackBy(amount);
}

void clearAllLetters() {
  for (uint8_t letter = 0; letter < LETTER_COUNT; ++letter) {
    fillLetter(letter, CRGB::Black);
  }
}

int underlineSegmentStart(uint8_t letterIndex) {
  return (letterIndex * NUM_U) / LETTER_COUNT;
}

int underlineSegmentEnd(uint8_t letterIndex) {
  return ((letterIndex + 1) * NUM_U) / LETTER_COUNT;
}

void fillUnderlineSegment(uint8_t letterIndex, CRGB color) {
  int start = underlineSegmentStart(letterIndex);
  int end = underlineSegmentEnd(letterIndex);
  for (int i = start; i < end; ++i) Strip_U[i] = color;
}

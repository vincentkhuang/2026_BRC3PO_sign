#include "Shows.h"

#include "ColorDropPattern.h"
#include "Controls.h"
#include "DancePattern.h"
#include "DiscoStrobePattern.h"
#include "LedHardware.h"
#include "LetterPatterns.h"
#include "NewPatternsShow.h"
#include "PalettePattern.h"

namespace {
constexpr unsigned long FIRST_SHOW_DURATION_MS = 120UL * 1000UL;
constexpr unsigned long SECOND_SHOW_DURATION_MS = 60UL * 1000UL;
constexpr uint8_t SECOND_SHOW_FPS = 100;

void delayToSyncFrameRate(uint8_t framesPerSecond) {
  static uint32_t previousMillis = 0;
  uint32_t currentMillis = millis();
  uint16_t elapsed = currentMillis - previousMillis;
  uint16_t target = 1000 / framesPerSecond;
  if (elapsed < target) delay(target - elapsed);
  previousMillis = millis();
}

void flashLetters(int count) {
  for (int i = 0; i < count; ++i) {
    setLetterColor(CRGB::White);
    LEDS.show();
    LEDS.delay(200);
    setLetterColor(CRGB::Black);
    LEDS.show();
    LEDS.delay(400);
  }
}

void runFirstShow() {
  currentBlending = LINEARBLEND;
  unsigned long startTime = millis();
  static uint8_t paletteIndex = 0;

  while (millis() - startTime < FIRST_SHOW_DURATION_MS) {
    renderPalettePattern(paletteIndex++);
    LEDS.show();
    LEDS.delay(200);
  }

  readControls();
  setLetterColor(currentColor);
  fill_solid(Strip_F, NUM_F, CRGB::Red);
  fill_solid(Strip_U, NUM_U, CRGB::White);
  LEDS.show();
  LEDS.delay(200);

  runDancePattern(10);
  fill_solid(Strip_U, NUM_U, CRGB::Black);
}

void runSecondShow() {
  setLetterColor(CRGB::Black);
  fill_solid(Strip_F, NUM_F, CRGB::Black);
  fill_solid(Strip_U, NUM_U, CRGB::Black);
  LEDS.show();
  LEDS.delay(500);
  flashLetters(3);

  int letterPaletteIndex = 0;
  static int dropColorIndex = 0;
  unsigned long startTime = millis();

  while (millis() - startTime < SECOND_SHOW_DURATION_MS) {
    EVERY_N_SECONDS(10) { dropColorIndex += 3; }
    if (dropColorIndex > 12) dropColorIndex = 0;

    renderColorDropPattern(dropColorIndex);
    letterColorShift(letterPaletteIndex);
    renderDiscoStrobePattern();
    LEDS.show();
    delayToSyncFrameRate(SECOND_SHOW_FPS);

    EVERY_N_SECONDS(1) { ++letterPaletteIndex; }
    if (letterPaletteIndex > 15) letterPaletteIndex = 0;
  }

  flashLetters(5);
}
}  // namespace

void runLegacyFirstShow() {
  runFirstShow();
}

void runLegacySecondShow() {
  runSecondShow();
}

void updateShows() {
  static bool startupPending = true;
  static uint8_t nextShow = 0;

  if (startupPending) {
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    if (RUN_NEW_PATTERN_PREVIEW_AT_STARTUP) runNewPatternsShow();
    runLegacyFirstShow();
    runLegacySecondShow();
    startupPending = false;
  }

  EVERY_N_MINUTES(10) {
    switch (nextShow) {
      case 0:
        runLegacyFirstShow();
        break;
      case 1:
        runLegacySecondShow();
        break;
      default:
        runNewPatternsShow();
        break;
    }
    nextShow = (nextShow + 1) % 3;
  }
}

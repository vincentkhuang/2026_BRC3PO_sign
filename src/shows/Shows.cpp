#include "Shows.h"

#include "../core/Config.h"
#include "../core/Controls.h"
#include "../core/LedHardware.h"
#include "../core/SignTransitions.h"
#include "../patterns/ColorDropPattern.h"
#include "../patterns/DancePattern.h"
#include "../patterns/DiscoStrobePattern.h"
#include "../patterns/IdleScannerPattern.h"
#include "../patterns/LetterPatterns.h"
#include "../patterns/PalettePattern.h"
#include "NewPatternsShow.h"

namespace {
constexpr uint8_t FIRST_SHOW_TIME_SCALE_PERCENT = 49;
constexpr uint8_t SECOND_SHOW_TIME_SCALE_PERCENT = 70;
constexpr unsigned long FIRST_SHOW_DURATION_MS =
    120UL * 1000UL * FIRST_SHOW_TIME_SCALE_PERCENT / 100UL;
constexpr unsigned long SECOND_SHOW_DURATION_MS =
    60UL * 1000UL * SECOND_SHOW_TIME_SCALE_PERCENT / 100UL;
constexpr uint16_t SECOND_SHOW_FRAME_MS =
    (1000UL / 100UL) * SECOND_SHOW_TIME_SCALE_PERCENT / 100UL;
constexpr unsigned long DROP_COLOR_INTERVAL_MS =
    10UL * 1000UL * SECOND_SHOW_TIME_SCALE_PERCENT / 100UL;
constexpr unsigned long LETTER_COLOR_INTERVAL_MS =
    1000UL * SECOND_SHOW_TIME_SCALE_PERCENT / 100UL;
constexpr uint8_t LEGACY_FADE_FRAME_COUNT = 24;

void legacyDelay(unsigned long milliseconds, uint8_t timeScalePercent) {
  LEDS.delay(milliseconds * timeScalePercent / 100UL);
}

void delayToSyncFrameRate(uint16_t targetFrameMillis) {
  static uint32_t previousMillis = 0;
  uint32_t currentMillis = millis();
  uint16_t elapsed = currentMillis - previousMillis;
  if (elapsed < targetFrameMillis) delay(targetFrameMillis - elapsed);
  previousMillis = millis();
}

void flashLetters(int count, uint8_t timeScalePercent) {
  for (int i = 0; i < count; ++i) {
    setLetterColor(CRGB::White);
    LEDS.show();
    legacyDelay(200, timeScalePercent);
    setLetterColor(CRGB::Black);
    LEDS.show();
    legacyDelay(400, timeScalePercent);
  }
}

void runFirstShow() {
  currentBlending = LINEARBLEND;
  static uint8_t paletteIndex = 0;

  captureSignTransitionSource();
  renderPalettePattern(paletteIndex++);
  fadeIntoCurrentSignFrame(LEGACY_FADE_FRAME_COUNT, 25,
                           FIRST_SHOW_TIME_SCALE_PERCENT);
  unsigned long startTime = millis();

  while (millis() - startTime < FIRST_SHOW_DURATION_MS) {
    renderPalettePattern(paletteIndex++);
    LEDS.show();
    legacyDelay(200, FIRST_SHOW_TIME_SCALE_PERCENT);
  }

  readControls();
  setLetterColor(currentColor);
  fill_solid(Strip_F, NUM_F, CRGB::Red);
  fill_solid(Strip_U, NUM_U, CRGB::White);
  LEDS.show();
  legacyDelay(200, FIRST_SHOW_TIME_SCALE_PERCENT);

  runDancePattern(10, FIRST_SHOW_TIME_SCALE_PERCENT);
  fill_solid(Strip_U, NUM_U, CRGB::Black);
}

void runSecondShow() {
  captureSignTransitionSource();
  setLetterColor(CRGB::White);
  fill_solid(Strip_F, NUM_F, CRGB::Black);
  fill_solid(Strip_U, NUM_U, CRGB::Black);
  fadeIntoCurrentSignFrame(LEGACY_FADE_FRAME_COUNT, 25,
                           SECOND_SHOW_TIME_SCALE_PERCENT);
  legacyDelay(200, SECOND_SHOW_TIME_SCALE_PERCENT);
  captureSignTransitionSource();
  setLetterColor(CRGB::Black);
  fadeIntoCurrentSignFrame(LEGACY_FADE_FRAME_COUNT, 25,
                           SECOND_SHOW_TIME_SCALE_PERCENT);
  legacyDelay(400, SECOND_SHOW_TIME_SCALE_PERCENT);
  flashLetters(2, SECOND_SHOW_TIME_SCALE_PERCENT);

  int letterPaletteIndex = 0;
  static int dropColorIndex = 0;
  unsigned long startTime = millis();

  while (millis() - startTime < SECOND_SHOW_DURATION_MS) {
    EVERY_N_MILLISECONDS(DROP_COLOR_INTERVAL_MS) { dropColorIndex += 3; }
    if (dropColorIndex > 12) dropColorIndex = 0;

    renderColorDropPattern(dropColorIndex);
    letterColorShift(letterPaletteIndex);
    renderDiscoStrobePattern();
    LEDS.show();
    delayToSyncFrameRate(SECOND_SHOW_FRAME_MS);

    EVERY_N_MILLISECONDS(LETTER_COLOR_INTERVAL_MS) { ++letterPaletteIndex; }
    if (letterPaletteIndex > 15) letterPaletteIndex = 0;
  }

  flashLetters(5, SECOND_SHOW_TIME_SCALE_PERCENT);
}
}  // namespace

void runLegacyFirstShow() {
  runFirstShow();
}

void runLegacySecondShow() {
  runSecondShow();
}

void updateShows() {
  static bool schedulerInitialized = false;
  static uint8_t nextShow = 0;
  static unsigned long idleStartedAt = 0;

  if (!schedulerInitialized) {
    idleStartedAt = millis();
    schedulerInitialized = true;
    return;
  }

  if (millis() - idleStartedAt < IDLE_SCANNER_DURATION_MS) return;

  switch (nextShow) {
    case 0:
      runNewPatternsShow();
      break;
    case 1:
      runLegacyFirstShow();
      break;
    default:
      runLegacySecondShow();
      break;
  }
  nextShow = (nextShow + 1) % 3;

  // Resume the resting animation for a complete configured interval before
  // the next show. Reinitialization also fades smoothly into Idle Scanner.
  initIdleScannerPattern();
  idleStartedAt = millis();
}

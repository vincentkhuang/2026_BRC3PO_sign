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
CRGB legacyFadeSource[NUM_LEDS];
CRGB legacyFadeTarget[NUM_LEDS];

uint8_t blendChannel(uint8_t start, uint8_t target, uint8_t amount) {
  return static_cast<uint8_t>(
      (static_cast<uint16_t>(start) * (255 - amount) +
       static_cast<uint16_t>(target) * amount + 127) /
      255);
}

CRGB blendColor(CRGB start, CRGB target, uint8_t amount) {
  return CRGB(blendChannel(start.r, target.r, amount),
              blendChannel(start.g, target.g, amount),
              blendChannel(start.b, target.b, amount));
}

void legacyDelay(unsigned long milliseconds, uint8_t timeScalePercent) {
  LEDS.delay(milliseconds * timeScalePercent / 100UL);
}

void captureLegacyFadeSource() {
  for (int pixel = 0; pixel < NUM_LEDS; ++pixel) {
    legacyFadeSource[pixel] = leds[pixel];
  }
}

void fadeIntoCurrentFrame(uint8_t timeScalePercent) {
  for (int pixel = 0; pixel < NUM_LEDS; ++pixel) {
    legacyFadeTarget[pixel] = leds[pixel];
  }

  for (uint8_t frame = 1; frame <= LEGACY_FADE_FRAME_COUNT; ++frame) {
    uint8_t amount =
        static_cast<uint8_t>((static_cast<uint16_t>(frame) * 255) /
                             LEGACY_FADE_FRAME_COUNT);
    for (int pixel = 0; pixel < NUM_LEDS; ++pixel) {
      leds[pixel] =
          blendColor(legacyFadeSource[pixel], legacyFadeTarget[pixel], amount);
    }
    LEDS.show();
    legacyDelay(25, timeScalePercent);
  }
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

  captureLegacyFadeSource();
  renderPalettePattern(paletteIndex++);
  fadeIntoCurrentFrame(FIRST_SHOW_TIME_SCALE_PERCENT);
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
  captureLegacyFadeSource();
  setLetterColor(CRGB::White);
  fill_solid(Strip_F, NUM_F, CRGB::Black);
  fill_solid(Strip_U, NUM_U, CRGB::Black);
  fadeIntoCurrentFrame(SECOND_SHOW_TIME_SCALE_PERCENT);
  legacyDelay(200, SECOND_SHOW_TIME_SCALE_PERCENT);
  setLetterColor(CRGB::Black);
  LEDS.show();
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

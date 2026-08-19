#include <cstdlib>
#include <iostream>
#include <string>

#include "src/core/Controls.h"
#include "src/core/LedHardware.h"
#include "src/core/SignTransitions.h"
#include "src/patterns/BlackoutRevealPattern.h"
#include "src/patterns/ColorDropPattern.h"
#include "src/patterns/CometLettersPattern.h"
#include "src/patterns/DancePattern.h"
#include "src/patterns/DiscoStrobePattern.h"
#include "src/patterns/FadeOutPattern.h"
#include "src/patterns/IdleScannerPattern.h"
#include "src/patterns/LetterDominoPattern.h"
#include "src/patterns/LetterPatterns.h"
#include "src/patterns/PalettePattern.h"
#include "src/patterns/PowerUpPattern.h"
#include "src/patterns/ProtocolGoldPattern.h"
#include "src/patterns/SignalRelayPattern.h"
#include "src/patterns/TypewriterPattern.h"
#include "src/shows/NewPatternsShow.h"
#include "src/shows/Shows.h"

namespace {
struct PatternEntry {
  const char* name;
  void (*run)();
};

constexpr uint8_t LEGACY_FIRST_PREVIEW_TIME_SCALE_PERCENT = 49;
constexpr uint8_t LEGACY_SECOND_PREVIEW_TIME_SCALE_PERCENT = 70;
constexpr uint16_t LEGACY_SECOND_PREVIEW_FRAME_MS = 7;
constexpr unsigned long LEGACY_SECOND_PREVIEW_DURATION_MS = 8000;

void legacyPreviewDelay(unsigned long milliseconds,
                        uint8_t timeScalePercent) {
  LEDS.delay(milliseconds * timeScalePercent / 100UL);
}

void runIdleScannerPreview() {
  // Record one complete out-and-back sweep. The Teensy runs this renderer
  // continuously between scheduled shows; a single sweep keeps the local
  // preview useful without producing a seven-minute recording.
  constexpr uint16_t IDLE_SCANNER_PREVIEW_STEPS = 2 * (NUM_U - 1) + 1;
  initIdleScannerPattern();
  for (uint16_t step = 0; step < IDLE_SCANNER_PREVIEW_STEPS; ++step) {
    renderIdleScannerPattern();
  }
}

void runPalettePreview() {
  currentBlending = LINEARBLEND;
  captureSignTransitionSource();
  renderPalettePattern(0);
  fadeIntoCurrentSignFrame(24, 25,
                           LEGACY_FIRST_PREVIEW_TIME_SCALE_PERCENT);

  for (uint8_t colorIndex = 1; colorIndex < 96; ++colorIndex) {
    renderPalettePattern(colorIndex);
    LEDS.show();
    legacyPreviewDelay(200, LEGACY_FIRST_PREVIEW_TIME_SCALE_PERCENT);
  }
}

void runDancePreview() {
  readControls();
  captureSignTransitionSource();
  setLetterColor(currentColor);
  fill_solid(Strip_F, NUM_F, CRGB::Red);
  fill_solid(Strip_U, NUM_U, CRGB::White);
  fadeIntoCurrentSignFrame(24, 25,
                           LEGACY_FIRST_PREVIEW_TIME_SCALE_PERCENT);
  legacyPreviewDelay(200, LEGACY_FIRST_PREVIEW_TIME_SCALE_PERCENT);
  runDancePattern(10, LEGACY_FIRST_PREVIEW_TIME_SCALE_PERCENT);
}

void runColorDropPreview() {
  unsigned long startedAt = millis();
  while (millis() - startedAt < LEGACY_SECOND_PREVIEW_DURATION_MS) {
    int colorIndex =
        static_cast<int>((millis() - startedAt) / 7000UL) * 3;
    renderColorDropPattern(colorIndex);
    LEDS.show();
    LEDS.delay(LEGACY_SECOND_PREVIEW_FRAME_MS);
  }
}

void runDiscoStrobePreview() {
  unsigned long startedAt = millis();
  while (millis() - startedAt < LEGACY_SECOND_PREVIEW_DURATION_MS) {
    renderDiscoStrobePattern();
    LEDS.show();
    LEDS.delay(LEGACY_SECOND_PREVIEW_FRAME_MS);
  }
}

const PatternEntry PATTERNS[] = {
    {"show", runNewPatternsShow},
    {"legacy-first", runLegacyFirstShow},
    {"legacy-second", runLegacySecondShow},
    {"idle-scanner", runIdleScannerPreview},
    {"power-up", runPowerUpPattern},
    {"typewriter", runTypewriterPattern},
    {"signal-relay", runSignalRelayPattern},
    {"letter-domino", runLetterDominoPattern},
    {"comet-letters", runCometLettersPattern},
    {"blackout-reveal", runBlackoutRevealPattern},
    {"protocol-gold", runProtocolGoldPattern},
    {"fade-out", runFadeOutPattern},
    {"palette", runPalettePreview},
    {"dance", runDancePreview},
    {"color-drop", runColorDropPreview},
    {"disco-strobe", runDiscoStrobePreview},
};

void printUsage(const char* executable) {
  std::cerr << "Usage: " << executable
            << " <pattern> <output.bin> [seed]\nPatterns:";
  for (const PatternEntry& pattern : PATTERNS) {
    std::cerr << " " << pattern.name;
  }
  std::cerr << "\n";
}
}  // namespace

int main(int argc, char** argv) {
  if (argc < 3 || argc > 4) {
    printUsage(argv[0]);
    return 2;
  }

  const std::string requestedPattern = argv[1];
  const std::string outputPath = argv[2];
  uint32_t seed = argc == 4
                      ? static_cast<uint32_t>(std::strtoul(argv[3], nullptr, 10))
                      : 42;

  const PatternEntry* selected = nullptr;
  for (const PatternEntry& pattern : PATTERNS) {
    if (requestedPattern == pattern.name) {
      selected = &pattern;
      break;
    }
  }
  if (!selected) {
    printUsage(argv[0]);
    return 2;
  }

  simSetMillis(seed);
  simSetRandomSeed(seed);
  initLedHardware();
  LEDS.resetCapture();
  LEDS.show();
  selected->run();

  if (!LEDS.writeRecording(outputPath)) {
    std::cerr << "Could not write recording: " << outputPath << "\n";
    return 1;
  }

  std::cout << requestedPattern << ": " << LEDS.frameCount() << " frames, "
            << LEDS.durationMillis() << " ms\n";
  return 0;
}

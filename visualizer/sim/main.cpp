#include <cstdlib>
#include <iostream>
#include <string>

#include "src/core/LedHardware.h"
#include "src/patterns/BlackoutRevealPattern.h"
#include "src/patterns/CometLettersPattern.h"
#include "src/patterns/FadeOutPattern.h"
#include "src/patterns/IdleScannerPattern.h"
#include "src/patterns/LetterDominoPattern.h"
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

const PatternEntry PATTERNS[] = {
    {"show", runNewPatternsShow},
    {"idle-scanner", runIdleScannerPreview},
    {"power-up", runPowerUpPattern},
    {"typewriter", runTypewriterPattern},
    {"signal-relay", runSignalRelayPattern},
    {"letter-domino", runLetterDominoPattern},
    {"comet-letters", runCometLettersPattern},
    {"blackout-reveal", runBlackoutRevealPattern},
    {"protocol-gold", runProtocolGoldPattern},
    {"fade-out", runFadeOutPattern},
    {"legacy-first", runLegacyFirstShow},
    {"legacy-second", runLegacySecondShow},
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

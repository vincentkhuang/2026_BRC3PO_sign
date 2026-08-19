#include <cstdlib>
#include <iostream>
#include <string>

#include "BlackoutRevealPattern.h"
#include "CometLettersPattern.h"
#include "FadeOutPattern.h"
#include "LedHardware.h"
#include "LetterDominoPattern.h"
#include "NewPatternsShow.h"
#include "PowerUpPattern.h"
#include "ProtocolGoldPattern.h"
#include "SignalRelayPattern.h"
#include "TypewriterPattern.h"

namespace {
struct PatternEntry {
  const char* name;
  void (*run)();
};

const PatternEntry PATTERNS[] = {
    {"show", runNewPatternsShow},
    {"power-up", runPowerUpPattern},
    {"typewriter", runTypewriterPattern},
    {"signal-relay", runSignalRelayPattern},
    {"letter-domino", runLetterDominoPattern},
    {"comet-letters", runCometLettersPattern},
    {"blackout-reveal", runBlackoutRevealPattern},
    {"protocol-gold", runProtocolGoldPattern},
    {"fade-out", runFadeOutPattern},
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

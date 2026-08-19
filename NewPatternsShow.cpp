#include "NewPatternsShow.h"

#include "BlackoutRevealPattern.h"
#include "CometLettersPattern.h"
#include "FadeOutPattern.h"
#include "LetterDominoPattern.h"
#include "LedHardware.h"
#include "PowerUpPattern.h"
#include "ProtocolGoldPattern.h"
#include "SignalRelayPattern.h"
#include "TypewriterPattern.h"

namespace {
enum KineticPattern : uint8_t {
  SIGNAL_RELAY,
  LETTER_DOMINO,
  COMET_LETTERS,
  KINETIC_PATTERN_COUNT
};

void shuffleKineticPatterns(uint8_t order[KINETIC_PATTERN_COUNT]) {
  static uint8_t previousLastPattern = KINETIC_PATTERN_COUNT;

  for (uint8_t i = 0; i < KINETIC_PATTERN_COUNT; ++i) order[i] = i;

  for (int i = KINETIC_PATTERN_COUNT - 1; i > 0; --i) {
    uint8_t swapIndex = random8(i + 1);
    uint8_t value = order[i];
    order[i] = order[swapIndex];
    order[swapIndex] = value;
  }

  // Avoid ending one show and beginning the next kinetic section with the
  // same pattern, while preserving a complete three-pattern shuffle.
  if (order[0] == previousLastPattern) {
    uint8_t value = order[0];
    order[0] = order[1];
    order[1] = value;
  }
  previousLastPattern = order[KINETIC_PATTERN_COUNT - 1];
}

void runKineticPattern(uint8_t pattern) {
  switch (pattern) {
    case SIGNAL_RELAY:
      runSignalRelayPattern();
      break;
    case LETTER_DOMINO:
      runLetterDominoPattern();
      break;
    default:
      runCometLettersPattern();
      break;
  }
}
}  // namespace

void runNewPatternsShow() {
  random16_add_entropy(micros());

  runPowerUpPattern();
  runTypewriterPattern();

  uint8_t kineticOrder[KINETIC_PATTERN_COUNT];
  shuffleKineticPatterns(kineticOrder);
  for (uint8_t i = 0; i < KINETIC_PATTERN_COUNT; ++i) {
    runKineticPattern(kineticOrder[i]);
  }

  runBlackoutRevealPattern();
  runProtocolGoldPattern();
  runFadeOutPattern();
}

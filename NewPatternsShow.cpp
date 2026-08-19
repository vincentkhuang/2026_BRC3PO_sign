#include "NewPatternsShow.h"

#include "BlackoutRevealPattern.h"
#include "CometLettersPattern.h"
#include "LetterDominoPattern.h"
#include "PowerUpPattern.h"
#include "ProtocolGoldPattern.h"
#include "SignalRelayPattern.h"
#include "TypewriterPattern.h"

void runNewPatternsShow() {
  runPowerUpPattern();
  runTypewriterPattern();
  runProtocolGoldPattern();
  runSignalRelayPattern();
  runLetterDominoPattern();
  runCometLettersPattern();
  runBlackoutRevealPattern();
}


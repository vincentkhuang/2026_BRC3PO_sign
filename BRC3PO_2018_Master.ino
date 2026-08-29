// BRC3PO continuous rainbow-comet diagnostic
// Teensy + OctoWS2811 + FastLED

#include "src/core/LedHardware.h"
#include "src/core/TeensyController.h"
#include "src/patterns/CometLettersPattern.h"

void setup() {
  initTeensyController();
  initLedHardware();
}

void loop() {
  runCometLettersPattern();
}

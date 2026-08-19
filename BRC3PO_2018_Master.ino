// BRC3PO sign controller
// Teensy + OctoWS2811 + FastLED

#include "src/patterns/IdleScannerPattern.h"
#include "src/core/LedHardware.h"
#include "src/core/TeensyController.h"
#include "src/shows/Shows.h"

void setup() {
  initTeensyController();
  initLedHardware();
  initIdleScannerPattern();
}

void loop() {
  renderIdleScannerPattern();
  updateShows();
}

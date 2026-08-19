// BRC3PO sign controller
// Teensy + OctoWS2811 + FastLED

#include "IdleScannerPattern.h"
#include "LedHardware.h"
#include "Shows.h"
#include "TeensyController.h"

void setup() {
  initTeensyController();
  initLedHardware();
  initIdleScannerPattern();
}

void loop() {
  renderIdleScannerPattern();
  updateShows();
}

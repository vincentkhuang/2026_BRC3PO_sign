// BRC3PO strip-count diagnostic
// Teensy + OctoWS2811 + FastLED

#include "src/core/LedHardware.h"
#include "src/core/TeensyController.h"
#include "src/patterns/StripCountTest.h"

void setup() {
  initTeensyController();
  initLedHardware();
}

void loop() {
  renderStripCountTest();
  delay(1000);
}

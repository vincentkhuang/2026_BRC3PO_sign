#include "LedHardware.h"

CRGBArray<NUM_LEDS> leds;

CRGBSet Strip_B(leds(B_START, B_START + NUM_B - 1));
CRGBSet Strip_R(leds(R_START, R_START + NUM_R - 1));
CRGBSet Strip_C(leds(C_START, C_START + NUM_C - 1));
CRGBSet Strip_3(leds(THREE_START, THREE_START + NUM_3 - 1));
CRGBSet Strip_P(leds(P_START, P_START + NUM_P - 1));
CRGBSet Strip_O(leds(O_START, O_START + NUM_O - 1));
CRGBSet Strip_U(leds(U_START, U_START + NUM_U - 1));
CRGBSet Strip_F(leds(F_START, F_START + NUM_F - 1));

CRGBPalette16 currentPalette;
TBlendType currentBlending;

void initLedHardware() {
  LEDS.addLeds<OCTOWS2811>(leds, NUM_LEDS_PER_STRIP);
  LEDS.setBrightness(INITIAL_BRIGHTNESS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}


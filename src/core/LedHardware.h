#pragma once

#define USE_OCTOWS2811
#include <OctoWS2811.h>
#include <FastLED.h>

#include "Config.h"

FASTLED_USING_NAMESPACE

extern CRGBArray<NUM_LEDS> leds;

extern CRGBSet Strip_B;
extern CRGBSet Strip_R;
extern CRGBSet Strip_C;
extern CRGBSet Strip_3;
extern CRGBSet Strip_P;
extern CRGBSet Strip_O;
extern CRGBSet Strip_U;
extern CRGBSet Strip_F;

extern CRGBPalette16 currentPalette;
extern TBlendType currentBlending;

void initLedHardware();


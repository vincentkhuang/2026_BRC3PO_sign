#include "ColorDropPattern.h"

#include "LedHardware.h"

namespace {
constexpr int COLOR_COUNT = 15;
CRGB colors[COLOR_COUNT] = {
    CRGB::Blue, CRGB::Yellow, CRGB::Red, CRGB::Purple, CRGB::Green,
    CRGB::Pink, CRGB::Teal, CRGB::Orange, CRGB::Cyan, CRGB::Magenta,
    CRGB::DarkBlue, CRGB::Gold, CRGB::Violet, CRGB::SeaGreen,
    CRGB::DarkRed};

CRGB colorAt(int index) {
  return colors[index % COLOR_COUNT];
}
}  // namespace

void renderColorDropPattern(int colorIndex) {
  constexpr int TOP_LED = 103;
  static int pos1 = TOP_LED;
  static int pos2 = pos1 + 33;
  static int pos3 = pos2 + 33;

  CRGB color1 = colorAt(colorIndex);
  CRGB color2 = colorAt(colorIndex + 1);
  CRGB color3 = colorAt(colorIndex + 2);

  fadeToBlackBy(Strip_F, NUM_F, 10);

  Strip_F[pos1] = CRGB::White;
  if (pos1 < TOP_LED) Strip_F[pos1 + 1] = color1;
  --pos1;
  if (pos1 < 0) {
    pos1 = TOP_LED;
    Strip_F[0] = color1;
    color1 = colorAt(colorIndex + 3);
  }

  --pos2;
  if (pos2 < 0) {
    pos2 = TOP_LED;
    Strip_F[0] = color2;
    color2 = colorAt(colorIndex + 4);
  }
  if (pos2 < TOP_LED) Strip_F[pos2] = CRGB::White;
  if (pos2 < TOP_LED - 1) Strip_F[pos2 + 1] = color2;

  --pos3;
  if (pos3 < 0) {
    pos3 = TOP_LED;
    Strip_F[0] = color3;
    color3 = colorAt(colorIndex + 5);
  }
  if (pos3 < TOP_LED) Strip_F[pos3] = CRGB::White;
  if (pos3 < TOP_LED - 1) Strip_F[pos3 + 1] = color3;
}


#include "FastLED.h"

#include <fstream>

namespace {
uint32_t randomState = 0xB3C3F00D;

uint8_t saturatingAdd(uint8_t left, uint8_t right) {
  uint16_t sum = static_cast<uint16_t>(left) + right;
  return static_cast<uint8_t>(sum > 255 ? 255 : sum);
}

CRGB hsvToRgb(const CHSV& hsv) {
  float hue = (hsv.hue / 255.0f) * 6.0f;
  float saturation = hsv.saturation / 255.0f;
  float value = hsv.value / 255.0f;
  int sector = static_cast<int>(hue) % 6;
  float fraction = hue - static_cast<int>(hue);
  float p = value * (1.0f - saturation);
  float q = value * (1.0f - fraction * saturation);
  float t = value * (1.0f - (1.0f - fraction) * saturation);
  float red = 0, green = 0, blue = 0;

  switch (sector) {
    case 0: red = value; green = t; blue = p; break;
    case 1: red = q; green = value; blue = p; break;
    case 2: red = p; green = value; blue = t; break;
    case 3: red = p; green = q; blue = value; break;
    case 4: red = t; green = p; blue = value; break;
    default: red = value; green = p; blue = q; break;
  }

  return CRGB(static_cast<uint8_t>(red * 255),
              static_cast<uint8_t>(green * 255),
              static_cast<uint8_t>(blue * 255));
}

CRGBPalette16 makeRainbowPalette() {
  CRGBPalette16 palette;
  for (uint8_t i = 0; i < 16; ++i) {
    palette.colors[i] = hsvToRgb(CHSV(i * 16, 255, 255));
  }
  return palette;
}

CRGBPalette16 makePartyPalette() {
  CRGBPalette16 palette;
  const CRGB party[] = {CRGB::Purple, CRGB::Magenta, CRGB::Blue,
                        CRGB::Cyan, CRGB::Green, CRGB::Yellow,
                        CRGB::Orange, CRGB::Red};
  for (uint8_t i = 0; i < 16; ++i) palette.colors[i] = party[i % 8];
  return palette;
}

void writeUint32(std::ofstream& output, uint32_t value) {
  const char bytes[4] = {
      static_cast<char>(value & 0xFF),
      static_cast<char>((value >> 8) & 0xFF),
      static_cast<char>((value >> 16) & 0xFF),
      static_cast<char>((value >> 24) & 0xFF)};
  output.write(bytes, 4);
}
}  // namespace

const CRGB CRGB::Black(0, 0, 0);
const CRGB CRGB::White(255, 255, 255);
const CRGB CRGB::Red(255, 0, 0);
const CRGB CRGB::DarkRed(139, 0, 0);
const CRGB CRGB::Gold(255, 180, 0);
const CRGB CRGB::Cyan(0, 255, 255);
const CRGB CRGB::Blue(0, 0, 255);
const CRGB CRGB::Yellow(255, 255, 0);
const CRGB CRGB::Purple(128, 0, 128);
const CRGB CRGB::Green(0, 255, 0);
const CRGB CRGB::Pink(255, 20, 147);
const CRGB CRGB::Teal(0, 128, 128);
const CRGB CRGB::Orange(255, 100, 0);
const CRGB CRGB::Magenta(255, 0, 255);
const CRGB CRGB::DarkBlue(0, 0, 139);
const CRGB CRGB::Violet(138, 43, 226);
const CRGB CRGB::SeaGreen(46, 139, 87);

CRGB::CRGB(const CHSV& hsv) { *this = hsvToRgb(hsv); }

CRGB& CRGB::operator=(const CHSV& hsv) {
  *this = hsvToRgb(hsv);
  return *this;
}

CRGB& CRGB::operator+=(const CRGB& other) {
  r = saturatingAdd(r, other.r);
  g = saturatingAdd(g, other.g);
  b = saturatingAdd(b, other.b);
  return *this;
}

CRGB& CRGB::nscale8(uint8_t scale) {
  r = static_cast<uint8_t>((static_cast<uint16_t>(r) * scale) >> 8);
  g = static_cast<uint8_t>((static_cast<uint16_t>(g) * scale) >> 8);
  b = static_cast<uint8_t>((static_cast<uint16_t>(b) * scale) >> 8);
  return *this;
}

CRGB& CRGB::nscale8_video(uint8_t scale) {
  auto videoScale = [scale](uint8_t channel) {
    if (channel == 0 || scale == 0) return static_cast<uint8_t>(0);
    return static_cast<uint8_t>(
        ((static_cast<uint16_t>(channel) * scale) >> 8) + 1);
  };
  r = videoScale(r);
  g = videoScale(g);
  b = videoScale(b);
  return *this;
}

CRGB& CRGB::fadeToBlackBy(uint8_t amount) {
  return nscale8(255 - amount);
}

const CRGBPalette16 RainbowColors_p = makeRainbowPalette();
const CRGBPalette16 PartyColors_p = makePartyPalette();

CRGB ColorFromPalette(const CRGBPalette16& palette, uint8_t index,
                      uint8_t brightness, TBlendType) {
  CRGB color = palette.colors[index >> 4];
  color.nscale8_video(brightness);
  return color;
}

SimulatorFastLED LEDS;

void SimulatorFastLED::attach(CRGB* pixels, int count) {
  pixels_ = pixels;
  pixelCount_ = count;
}

void SimulatorFastLED::setBrightness(uint8_t brightness) {
  brightness_ = brightness;
}

uint8_t SimulatorFastLED::getBrightness() const { return brightness_; }

void SimulatorFastLED::show() {
  if (!pixels_ || pixelCount_ <= 0) return;
  SimulatorFrame frame;
  frame.timeMillis = millis();
  frame.pixels.assign(pixels_, pixels_ + pixelCount_);
  frames_.push_back(std::move(frame));
}

void SimulatorFastLED::delay(unsigned long milliseconds) {
  ::delay(milliseconds);
}

void SimulatorFastLED::resetCapture() { frames_.clear(); }

bool SimulatorFastLED::writeRecording(const std::string& path) const {
  if (frames_.empty()) return false;
  std::ofstream output(path, std::ios::binary);
  if (!output) return false;

  const char magic[8] = {'B', 'R', 'C', '3', 'P', 'O', '1', '\0'};
  output.write(magic, sizeof(magic));
  writeUint32(output, static_cast<uint32_t>(pixelCount_));
  writeUint32(output, static_cast<uint32_t>(frames_.size()));
  writeUint32(output, durationMillis());
  writeUint32(output, brightness_);

  uint32_t startTime = frames_.front().timeMillis;
  for (const SimulatorFrame& frame : frames_) {
    writeUint32(output, frame.timeMillis - startTime);
    for (const CRGB& pixel : frame.pixels) {
      output.put(static_cast<char>(pixel.r));
      output.put(static_cast<char>(pixel.g));
      output.put(static_cast<char>(pixel.b));
    }
  }
  return static_cast<bool>(output);
}

size_t SimulatorFastLED::frameCount() const { return frames_.size(); }

uint32_t SimulatorFastLED::durationMillis() const {
  if (frames_.size() < 2) return 0;
  return frames_.back().timeMillis - frames_.front().timeMillis;
}

uint8_t scale8(uint8_t value, uint8_t scale) {
  return static_cast<uint8_t>(
      (static_cast<uint16_t>(value) * scale) >> 8);
}

uint8_t sin8(uint8_t phase) {
  constexpr double PI = 3.14159265358979323846;
  double radians = (phase / 255.0) * 2.0 * PI;
  return static_cast<uint8_t>((std::sin(radians) + 1.0) * 127.5);
}

uint8_t beat8(uint8_t bpm) {
  uint32_t beats = static_cast<uint32_t>(millis()) * bpm * 256ULL / 60000ULL;
  return static_cast<uint8_t>(beats & 0xFF);
}

uint8_t beatsin8(uint8_t bpm, uint8_t low, uint8_t high) {
  uint8_t wave = sin8(beat8(bpm));
  return low + scale8(wave, high - low);
}

uint8_t ease8InOutCubic(uint8_t value) {
  float x = value / 255.0f;
  float eased = x < 0.5f ? 4.0f * x * x * x
                         : 1.0f - std::pow(-2.0f * x + 2.0f, 3.0f) / 2.0f;
  return static_cast<uint8_t>(eased * 255.0f);
}

uint8_t cubicwave8(uint8_t value) {
  return ease8InOutCubic(sin8(value));
}

void simSetRandomSeed(uint32_t seed) {
  randomState = seed == 0 ? 0xB3C3F00D : seed;
}

void random16_add_entropy(uint16_t entropy) {
  randomState ^= static_cast<uint32_t>(entropy) << 16 | entropy;
  randomState = randomState * 1664525U + 1013904223U;
}

uint16_t random16(uint16_t high) {
  randomState ^= randomState << 13;
  randomState ^= randomState >> 17;
  randomState ^= randomState << 5;
  return high == 0 ? 0 : static_cast<uint16_t>(randomState % high);
}

uint8_t random8(uint8_t high) {
  return static_cast<uint8_t>(random16(high));
}

uint8_t random8(uint8_t low, uint8_t high) {
  return high <= low ? low : low + random8(high - low);
}

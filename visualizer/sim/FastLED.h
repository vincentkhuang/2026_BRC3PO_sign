#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

#include "Arduino.h"

#define FASTLED_USING_NAMESPACE
#define EVERY_N_SECONDS(period) if (true)
#define EVERY_N_MINUTES(period) if (true)

struct CHSV {
  uint8_t hue;
  uint8_t saturation;
  uint8_t value;
  CHSV(uint8_t h, uint8_t s, uint8_t v)
      : hue(h), saturation(s), value(v) {}
};

struct CRGB {
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;

  CRGB() = default;
  CRGB(uint8_t red, uint8_t green, uint8_t blue)
      : r(red), g(green), b(blue) {}
  CRGB(const CHSV& hsv);

  CRGB& operator=(const CHSV& hsv);
  CRGB& operator+=(const CRGB& other);
  CRGB& nscale8(uint8_t scale);
  CRGB& nscale8_video(uint8_t scale);
  CRGB& fadeToBlackBy(uint8_t amount);

  static const CRGB Black;
  static const CRGB White;
  static const CRGB Red;
  static const CRGB DarkRed;
  static const CRGB Gold;
  static const CRGB Cyan;
  static const CRGB Blue;
  static const CRGB Yellow;
  static const CRGB Purple;
  static const CRGB Green;
  static const CRGB Pink;
  static const CRGB Teal;
  static const CRGB Orange;
  static const CRGB Magenta;
  static const CRGB DarkBlue;
  static const CRGB Violet;
  static const CRGB SeaGreen;
};

struct CRGBSlice {
  CRGB* pixels;
  int length;
};

template <int Size>
struct CRGBArray {
  CRGB pixels[Size];

  CRGB& operator[](int index) { return pixels[index]; }
  const CRGB& operator[](int index) const { return pixels[index]; }
  CRGB* data() { return pixels; }

  CRGBSlice operator()(int start, int end) {
    return {pixels + start, end - start + 1};
  }
};

struct CRGBSet {
  CRGB* pixels;
  int length;

  CRGBSet(CRGBSlice slice) : pixels(slice.pixels), length(slice.length) {}
  CRGB& operator[](int index) { return pixels[index]; }
  const CRGB& operator[](int index) const { return pixels[index]; }
};

template <int Size>
inline void fill_solid(CRGBArray<Size>& target, int count, CRGB color) {
  for (int i = 0; i < count && i < Size; ++i) target[i] = color;
}

inline void fill_solid(CRGBSet& target, int count, CRGB color) {
  for (int i = 0; i < count && i < target.length; ++i) target[i] = color;
}

inline void fadeToBlackBy(CRGBSet& target, int count, uint8_t amount) {
  for (int i = 0; i < count && i < target.length; ++i) {
    target[i].fadeToBlackBy(amount);
  }
}

struct CRGBPalette16 {
  std::array<CRGB, 16> colors{};
  CRGB operator[](int index) const { return colors[index & 0x0F]; }
};

enum TBlendType { LINEARBLEND };

extern const CRGBPalette16 RainbowColors_p;
extern const CRGBPalette16 PartyColors_p;

CRGB ColorFromPalette(const CRGBPalette16& palette, uint8_t index,
                      uint8_t brightness, TBlendType blending);

struct SimulatorFrame {
  uint32_t timeMillis;
  std::vector<CRGB> pixels;
};

class SimulatorFastLED {
 public:
  template <typename Controller, int Size>
  void addLeds(CRGBArray<Size>& array, int) {
    attach(array.data(), Size);
  }

  void attach(CRGB* pixels, int count);
  void setBrightness(uint8_t brightness);
  uint8_t getBrightness() const;
  void show();
  void delay(unsigned long milliseconds);
  void resetCapture();
  bool writeRecording(const std::string& path) const;
  size_t frameCount() const;
  uint32_t durationMillis() const;

 private:
  CRGB* pixels_ = nullptr;
  int pixelCount_ = 0;
  uint8_t brightness_ = 255;
  std::vector<SimulatorFrame> frames_;
};

extern SimulatorFastLED LEDS;

uint8_t scale8(uint8_t value, uint8_t scale);
uint8_t sin8(uint8_t phase);
uint8_t beat8(uint8_t bpm);
uint8_t beatsin8(uint8_t bpm, uint8_t low, uint8_t high);
uint8_t ease8InOutCubic(uint8_t value);
uint8_t cubicwave8(uint8_t value);
uint8_t random8(uint8_t high);
uint8_t random8(uint8_t low, uint8_t high);
uint16_t random16(uint16_t high);
void random16_add_entropy(uint16_t entropy);
void simSetRandomSeed(uint32_t seed);

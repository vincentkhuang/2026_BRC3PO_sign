#pragma once

#include <cstdint>

using std::uint8_t;
using std::uint16_t;
using std::uint32_t;

constexpr int OUTPUT = 1;
constexpr int HIGH = 1;

namespace simulator {
inline uint64_t clockMillis = 0;
}

inline unsigned long millis() {
  return static_cast<unsigned long>(simulator::clockMillis);
}

inline unsigned long micros() {
  return static_cast<unsigned long>(simulator::clockMillis * 1000ULL);
}

inline void delay(unsigned long milliseconds) {
  simulator::clockMillis += milliseconds;
}

inline void simSetMillis(uint64_t milliseconds) {
  simulator::clockMillis = milliseconds;
}

inline int analogRead(uint8_t) { return 512; }

inline long map(long value, long inputMin, long inputMax,
                long outputMin, long outputMax) {
  return (value - inputMin) * (outputMax - outputMin) /
             (inputMax - inputMin) +
         outputMin;
}

inline void pinMode(uint8_t, int) {}
inline void digitalWrite(uint8_t, int) {}

struct SimulatorSerial {
  void begin(int) {}
  void println(const char*) {}
};

inline SimulatorSerial Serial;

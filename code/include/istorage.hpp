#pragma once
#include <cstdint>

struct Signal {
  uint8_t batch;
  uint16_t id;
  long code;
  uint16_t pulseLength;
};

class IStorage {
public:
  virtual ~IStorage() = default;
  virtual uint16_t saveSignal(Signal signal) = 0;
  virtual uint16_t getSignalCount() = 0;
  virtual Signal getSignal(int index) = 0;
  virtual void removeSignal(int index) = 0;
};

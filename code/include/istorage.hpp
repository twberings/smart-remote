#pragma once
#include <string>

struct Signal {
  std::string name;
  long code;
  uint8_t pulseLength;
};

class IStorage {
public:
  virtual ~IStorage() = default;
  virtual void saveSignal(Signal signal) = 0;
  virtual uint16_t getSignalCount() = 0;
  virtual Signal getSignal(int index) = 0;
  virtual void removeSignal(int index) = 0;
  virtual void renameSignal(int index, const std::string &newName) = 0;
};

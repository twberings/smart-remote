#include "remote.hpp"

void Remote::loadSignals(IStorage &storage) {
  signals.clear();
  uint16_t count = storage.getSignalCount();
  for (uint16_t i = 0; i < count; i++) {
    signals.push_back(storage.getSignal(i));
  }
}

void Remote::sendSignal(const char *name) {
  for (const auto &signal : signals) {
    if (signal.name == name) {
      rcSwitch.enableTransmit(0);
      rcSwitch.setPulseLength(signal.pulseLength);
      rcSwitch.send(signal.code, 24);
      return;
    }
  }
}

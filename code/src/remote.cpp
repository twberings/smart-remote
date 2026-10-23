#include "remote.hpp"

Remote::Remote(IStorage &storage, Led &led) : storage(storage), led(led) {}

void Remote::loadSignals() {
  signals.clear();
  uint16_t count = storage.getSignalCount();
  for (uint16_t i = 0; i < count; i++) {
    signals.push_back(storage.getSignal(i));
  }
}

bool Remote::sendSignal(const char *name) {
  if (isRecording) {
    return false;
  }
  for (const auto &signal : signals) {
    if (signal.name == name) {
      led.setCycleSpeed(200);
      led.setState(LedState::BLINKING);
      led.setTimeout(1000);
      rcSwitch.enableTransmit(0);
      rcSwitch.setPulseLength(signal.pulseLength);
      rcSwitch.send(signal.code, 24);
      return true;
    }
  }
  return false;
}

void Remote::recordSignal() {
  if (isRecording) {
    return;
  }
  led.setCycleSpeed(1000);
  led.setState(LedState::BREATHING);
  isRecording = true;
  rcSwitch.enableReceive(RECEIVE_PIN);
}

void Remote::update() {
  if (isRecording) {
    if (rcSwitch.available()) {
      unsigned long code = rcSwitch.getReceivedValue();
      int pulseLength = rcSwitch.getReceivedDelay();
      rcSwitch.resetAvailable();
      Signal newSignal;
      static int signalCount = 0;
      newSignal.name =
          std::string("New Signal ") + std::to_string(signalCount++);
      newSignal.code = code;
      newSignal.pulseLength = pulseLength;
      signals.push_back(newSignal);
      storage.saveSignal(newSignal);
      led.setState(LedState::OFF);
      isRecording = false;
    }
  }
}

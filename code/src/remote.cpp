#include "remote.hpp"

Remote::Remote(IStorage &storage, Led &led) : storage(storage), led(led) {}

bool Remote::begin() {
  radio.setSpiPin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
  radio.Init();
  if (!radio.getCC1101()) {
    return false;
  }
  radio.setMHZ(433.92);
  return true;
}

void Remote::loadSignals() {
  signals.clear();
  uint16_t count = storage.getSignalCount();
  for (uint16_t i = 0; i < count; i++) {
    signals.push_back(storage.getSignal(i));
  }
}

bool Remote::sendSignal(uint16_t id, uint8_t batch) {
  if (isRecording) {
    return false;
  }
  for (const auto &signal : signals) {
    if (signal.id == id && signal.batch == batch) {
      led.setCycleSpeed(200);
      led.setState(LedState::BLINKING);
      led.setTimeout(1000);
      rcSwitch.enableTransmit(0);
      rcSwitch.setPulseLength(signal.pulseLength);
      rcSwitch.send(signal.code, 24);
      Serial.print("Sent signal: ");
      Serial.print("Code: ");
      Serial.print(signal.code);
      Serial.print(", Pulse Length: ");
      Serial.println(signal.pulseLength);
      return true;
    }
  }
  return false;
}

void Remote::recordSignal(uint8_t batch) {
  if (isRecording) {
    return;
  }
  recordingBatch = batch;
  led.setCycleSpeed(1000);
  led.setState(LedState::BREATHING);
  isRecording = true;
  rcSwitch.enableReceive(RECEIVE_PIN);
  radio.SetRx();
}

void Remote::update() {
  if (isRecording) {
    if (rcSwitch.available()) {
      unsigned long code = rcSwitch.getReceivedValue();
      int pulseLength = rcSwitch.getReceivedDelay();
      rcSwitch.resetAvailable();

      Signal newSignal;
      newSignal.code = code;
      newSignal.pulseLength = pulseLength;
      newSignal.id = storage.saveSignal(newSignal);
      signals.push_back(newSignal);

      led.setState(LedState::OFF);
      isRecording = false;

      Serial.println("Recorded signal: ");
      Serial.print("Code: ");
      Serial.print(code);
      Serial.print(", Pulse Length: ");
      Serial.println(pulseLength);
    }
  }
}

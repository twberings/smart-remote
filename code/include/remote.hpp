#pragma once
#include "RCSwitch.h"
#include "SmartRC_CC1101.h"
#include "istorage.hpp"
#include "led.hpp"
#include <vector>

#define RECEIVE_PIN D3
#define SCK_PIN D8
#define MISO_PIN D9
#define MOSI_PIN D10
#define SS_PIN D0

class Remote {
public:
  Remote(IStorage &storage, Led &led);
  void loadSignals();
  bool sendSignal(uint16_t id, uint8_t batch = 0);
  void recordSignal(uint8_t batch = 0);
  bool begin();
  void update();

private:
  bool isRecording = false;
  uint8_t recordingBatch = 0;
  std::vector<Signal> signals;
  RCSwitch rcSwitch;
  SmartRC_CC1101 radio;
  IStorage &storage;
  Led &led;
};

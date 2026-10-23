#pragma once
#include "RCSwitch.h"
#include "SmartRC_CC1101.h"
#include "istorage.hpp"
#include "led.hpp"
#include <vector>

#define RECEIVE_PIN 2

class Remote {
public:
  Remote(IStorage &storage, Led &led);
  void loadSignals();
  bool sendSignal(const char *name);
  void recordSignal();
  void update();

private:
  bool isRecording = false;
  std::vector<Signal> signals;
  RCSwitch rcSwitch;
  SmartRC_CC1101 radio;
  IStorage &storage;
  Led &led;
};

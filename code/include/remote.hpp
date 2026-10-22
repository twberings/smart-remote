#pragma once
#include "RCSwitch.h"
#include "SmartRC_CC1101.h"
#include "istorage.hpp"
#include <vector>

class Remote {
public:
  void loadSignals(IStorage &storage);
  void sendSignal(const char *name);
  void recordSignal(const char *name);

private:
  std::vector<Signal> signals;
  RCSwitch rcSwitch;
  SmartRC_CC1101 radio;
};

#pragma once
#include "eeprom.hpp"
#include "led.hpp"

enum class ConnectionState {
  DISCONNECTED,
  CONNECTING,
  CONNECTED,
  PAIRING,
};

class ConnectionHandler {
public:
  ConnectionHandler(EEPROM &eeprom, Led &led);
  void startWifi();
  void update();

private:
  ConnectionState state = ConnectionState::DISCONNECTED;
  EEPROM &eeprom;
  Led &led;
};

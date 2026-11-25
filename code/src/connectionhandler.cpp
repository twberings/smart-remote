#include "connectionhandler.hpp"
#include "WiFi.h"

ConnectionHandler::ConnectionHandler(EEPROM &eeprom, Led &led)
    : eeprom(eeprom), led(led) {}

void ConnectionHandler::startWifi() {
  this->state = ConnectionState::CONNECTING;
  this->led.setState(LedState::BREATHING);
  this->led.setCycleSpeed(500);

  char ssid[WIFI_SSID_SIZE];
  char password[WIFI_PASSWORD_SIZE];
  this->eeprom.getWifiCredentials(ssid, password);

  WiFi.begin(ssid, password);
}

void ConnectionHandler::update() {
  switch (this->state) {
  case ConnectionState::DISCONNECTED:
    break;
  case ConnectionState::CONNECTING:
    if (WiFi.status() == WL_CONNECTED) {
      this->state = ConnectionState::CONNECTED;
      this->led.setState(LedState::OFF);
    }
    break;
  case ConnectionState::CONNECTED:
    break;
  case ConnectionState::PAIRING:
    break;
  }
}

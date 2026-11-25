// #include "WiFi.h"
#include "button.hpp"
#include "connectionhandler.hpp"
#include "eeprom.hpp"
#include "led.hpp"
#include "remote.hpp"
// #include "webapi.hpp"
#include "OThreadCLI.h"

Led led(D2);
Button button(D7);
EEPROM eeprom(0x53);
Remote remote(eeprom, led);
ConnectionHandler connectionHandler(eeprom, led);
// WebServer server(80);
// WebAPI webAPI(server, remote);

void setup() {
  OThread.begin();
  Serial.begin(115200);

  if (!Wire.begin()) {
    while (1) {
      Serial.println("Failed to initialize I2C");
      delay(1000);
    }
  }

  if (!eeprom.begin()) {
    while (1) {
      Serial.println("Failed to initialize EEPROM");
      delay(1000);
    }
  }

  if (!remote.begin()) {
    while (1) {
      Serial.println("Failed to initialize Remote");
      delay(1000);
    }
  }

  button.setOnPress([]() {
    led.setState(LedState::BREATHING);
    led.setTimeout(5000);
  });
  button.setOnHold([]() {
    led.setState(LedState::BLINKING);
    led.setTimeout(5000);
  }, 2000);

  remote.loadSignals();
  // connectionHandler.startWifi();
}

void loop() {
  // connectionHandler.update();
  led.update();
  button.update();
  remote.update();
  // webAPI.update();
}

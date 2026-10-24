#include "button.hpp"
#include "eeprom.hpp"
#include "led.hpp"
#include "remote.hpp"
#include <Arduino.h>

Led led(D2);
Button button(D7);
EEPROM eeprom(0x53);
Remote remote(eeprom, led);

void setup() {
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
  });
  button.setOnHold([]() {}, 2000);
}

void loop() {
  led.update();
  button.update();
  remote.update();
}

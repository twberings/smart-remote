#include "button.hpp"
#include "eeprom.hpp"
#include "led.hpp"
#include <Arduino.h>

Led led(D2);
Button button(D7);
EEPROM eeprom(0x53);

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

  button.setOnPress([]() {
    char ssid[WIFI_SSID_SIZE];
    char password[WIFI_PASSWORD_SIZE];
    eeprom.getWifiCredentials(ssid, password);
    Serial.print("SSID: ");
    Serial.println(ssid);
    Serial.print("Password: ");
    Serial.println(password);
  });
  button.setOnHold(
      []() {
        eeprom.saveWifiCredentials("MySSID", "MyPassword");
        Serial.println("Saved WiFi credentials to EEPROM");
      },
      2000);
}

void loop() {
  led.update();
  button.update();
}

#include "button.hpp"
#include "led.hpp"
#include <Arduino.h>

Led led(D2);
Button button(D7);
int counter = 0;

void setup() {
  Serial.begin(115200);
  button.setOnPress([]() {
    counter++;
    Serial.print("Button pressed! Counter: ");
    Serial.println(counter);
    if (counter % 4 == 0) {
      led.setState(LedState::OFF);
    } else if (counter % 4 == 1) {
      led.setState(LedState::ON);
    } else if (counter % 4 == 2) {
      led.setState(LedState::BLINKING);
      led.setCycleSpeed(500);
    } else if (counter % 4 == 3) {
      led.setState(LedState::BREATHING);
      led.setCycleSpeed(2000);
    }
  });
  button.setOnHold(
      []() {
        Serial.println("Resetting counter.");
        counter = 0;
        led.setState(LedState::OFF);
      },
      2000);
}

void loop() {
  led.update();
  button.update();
}

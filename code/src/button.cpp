#include "button.hpp"
#include "esp32-hal.h"
#include <Arduino.h>

Button::Button(int pin) : pin(pin) { pinMode(pin, INPUT_PULLDOWN); }

void Button::setOnPress(std::function<void()> callback) {
  this->onPress = callback;
}
void Button::setOnHold(std::function<void()> callback, unsigned long holdTime) {
  this->onHold = callback;
  this->holdTime = holdTime;
}

void Button::update() {
  static bool lastReading = LOW;
  static bool currentState = LOW;
  static unsigned long lastDebounceTime = 0;
  static unsigned long holding = 0;
  static bool held = false;
  const unsigned long debounceDelay = 50;
  bool reading = digitalRead(pin);

  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime >= debounceDelay) {
    if (reading != currentState) {
      currentState = reading;

      if (currentState == HIGH) {
        holding = millis();
      }

      if (currentState == LOW) {
        held = false;
        if (millis() - holding <= holdTime) {
          onPress();
        }
      }
    }
  }

  if (currentState == HIGH && millis() - holding >= holdTime && !held) {
    held = true;
    onHold();
  }

  lastReading = reading;
}

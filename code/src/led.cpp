#include "led.hpp"
#include <Arduino.h>

Led::Led(int pin) : pin(pin) { pinMode(pin, OUTPUT); }

void Led::setState(LedState state) { this->state = state; }
void Led::setCycleSpeed(int millis) { this->cycleSpeed = millis; }

void Led::update() {
  switch (state) {
  case LedState::OFF:
    analogWrite(pin, 0);
    break;
  case LedState::ON:
    analogWrite(pin, 255);
    break;
  case LedState::BLINKING:
    blink();
    break;
  case LedState::BREATHING:
    breathe();
    break;
  }
}

void Led::blink() {
  static unsigned long lastToggleTime = 0;
  static bool ledState = LOW;
  unsigned long currentTime = millis();
  if (currentTime - lastToggleTime >= cycleSpeed / 2) {
    ledState = !ledState;
    analogWrite(pin, ledState ? 255 : 0);
    lastToggleTime = currentTime;
  }
}

void Led::breathe() {
  static unsigned long lastUpdateTime = 0;
  static int brightness = 0;
  const int maxBrightness = 255;
  const int minBrightness = 0;
  static int fadeAmount = 5;
  static const unsigned int fadeTime = cycleSpeed / (2 * maxBrightness / fadeAmount);
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= fadeTime) {
    analogWrite(pin, brightness);
    brightness += fadeAmount;
    if (brightness <= minBrightness || brightness >= maxBrightness) {
      fadeAmount = -fadeAmount;
    }
    lastUpdateTime = currentTime;
  }
}

#pragma once

enum class LedState { OFF, ON, BLINKING, BREATHING };

class Led {
public:
  Led(int pin);
  void setState(LedState state);
  void setCycleSpeed(int millis);
  void setTimeout(int millis);
  void update();

private:
  int pin;
  LedState state = LedState::OFF;
  unsigned int cycleSpeed = 500;
  unsigned int timeout = 0;
  void blink();
  void breathe();
};

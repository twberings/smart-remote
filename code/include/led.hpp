#pragma once

enum class LedState { OFF, ON, BLINKING, BREATHING };

class Led {
public:
  Led(int pin);
  void setState(LedState state);
private:
  int pin;
  LedState state;
};

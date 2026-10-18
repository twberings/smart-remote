#pragma once

#include <functional>

class Button {
public:
  Button(int pin);
  void setOnPress(std::function<void()> callback);
  void setOnHold(std::function<void()> callback, unsigned long holdTime);
  void update();

private:
  int pin;
  unsigned long holdTime;
  std::function<void()> onPress;
  std::function<void()> onHold;
};

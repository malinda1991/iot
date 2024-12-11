#ifndef RELAY_MODULE_H
#define RELAY_MODULE_H

#include <Arduino.h>

enum RelayStatus
{
  on,
  off
};

class RelayModule
{
private:
  int pin;
  RelayStatus status;

public:
  RelayModule(int pin);
  void initialize();
  void turnOn();
  void turnOff();
  RelayStatus getStatus();
};

#endif
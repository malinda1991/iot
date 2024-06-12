#ifndef RELAY_MODULE_H
#define RELAY_MODULE_H

#include <Arduino.h>
#include "CommonParams.h"

class RelayModule{
  private:
    int pin;
    bool isTurnedOn;

  public:
    RelayModule(int pin);
    void initialize();
    void turnOn();
    void turnOff();
};

#endif
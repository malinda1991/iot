#ifndef DOOR_SENSOR_H
#define DOOR_SENSOR_H

#include <Arduino.h>
#include "CommonParams.h"

class DoorSensor{
  private:
    byte pin;
    int doorState;
    
  public:
    DoorSensor(byte pin);
    void initialize();
    GarageDoor getDoorState();
};

#endif
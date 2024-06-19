#ifndef DOOR_SENSOR_H
#define DOOR_SENSOR_H

#include <Arduino.h>
#include "CommonParams.h"

enum SensorType{
  PRIMARY,
  SECONDARY
}

struct DeviceError {
  bool hasError;
  String errorMessage;
}

class DoorSensor{
  private:
    byte pin;
    int doorState;
    SensorType type;
    static DoorSensor primary;
    static DoorSensor secondary;
    static DeviceError error;
    static setErrorInfo(DeviceError error);
    
  public:
    DoorSensor(byte pin, SensorType type);
    void initialize();
    GarageDoor getDoorState();
    static void validateDevices();
};

#endif
#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H

#include <Arduino.h>
#include "CommonParams.h"

class UltrasonicSensor{
  private:
    int triggerPin;
    int echoPin;
    float duration;
    float distance;

  public:
    UltrasonicSensor(int triggerPin, int echoPin);
    void initialize();
    void updateSensorData();
    float getDistance();
};

#endif
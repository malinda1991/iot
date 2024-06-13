#ifndef AIR_QUALITY_SENSOR_H
#define AIR_QUALITY_SENSOR_H

#include <Arduino.h>
#include "CommonParams.h"

class AirQualitySensor{
  private:
    int analogPin;
    int sensorData;

  public:
    AirQualitySensor(int analogPin);
    void initialize();
    void updateSensorData();
    int getAirQualityLevel();
};

#endif
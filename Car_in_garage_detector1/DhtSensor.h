#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include <Arduino.h>
#include <dht11.h>
#include "CommonParams.h"

struct DhtData {
  int temperature;
  int humidity;
};

class DhtSensor{
  private:
    byte pin;
    int dhtState;
    dht11 DHT11;

  public:
    DhtSensor(byte pin);
    void initialize();
    void updateSensorData();
    int getTemperature();
    int getHumidity();
};

#endif
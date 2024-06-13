#include "DhtSensor.h"
#include <dht11.h>
#include <Arduino.h>

DhtSensor::DhtSensor(
  byte pin) {
  dht11 DHT11;
  this->pin = pin;
  this->DHT11 = DHT11;
}

void DhtSensor::initialize() {
  pinMode(this->pin, INPUT);
}

void DhtSensor::updateSensorData() {
  this->dhtState = this->DHT11.read(this->pin);
}

int DhtSensor::getTemperature() {
  if (this->DHT11.temperature) {
    return (float)this->DHT11.temperature;
  }
  return 0.0;
}

int DhtSensor::getHumidity(){
  if(this->DHT11.humidity){
    return (float)this->DHT11.humidity;
  }
  return 0.0;
}
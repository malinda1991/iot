#include "AirQualitySensor.h"

AirQualitySensor::AirQualitySensor(int analogPin){
  this->analogPin = analogPin;
}

void AirQualitySensor::initialize(){
  pinMode(this->analogPin, INPUT);
}

void AirQualitySensor::updateSensorData(){
  this->sensorData = analogRead(this->analogPin);
}

int AirQualitySensor::getAirQualityLevel(){
  return this->sensorData;
}
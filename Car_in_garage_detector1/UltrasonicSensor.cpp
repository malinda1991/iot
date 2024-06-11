#include "Arduino.h"
#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(int triggerPin, int echoPin) {
  this->triggerPin = triggerPin;
  this->echoPin = echoPin;
  this->duration = 0;
  this->distance = 0;
}

void UltrasonicSensor::initialize() {
  pinMode(this->triggerPin, OUTPUT);
  pinMode(this->echoPin, INPUT);
}

void UltrasonicSensor::updateSensorData() {
  digitalWrite(this->triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(this->triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(this->triggerPin, LOW);
  this->duration = pulseIn(this->echoPin, HIGH);
  this->distance = (this->duration * .0343) / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
}

float UltrasonicSensor::getDistance() {
  return this->distance;
}
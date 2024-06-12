#include "RelayModule.h"

RelayModule::RelayModule(int pin) {
  this->pin = pin;
  this->isTurnedOn = false;
}

void RelayModule::initialize() {
  pinMode(this->pin, OUTPUT);
}

void RelayModule::turnOn(){
  digitalWrite(this->pin, LOW);  // LOW means on
  this->isTurnedOn = true;
}

void RelayModule::turnOff(){
  digitalWrite(this->pin, HIGH);  // HIGH means off
  this->isTurnedOn = false;
}
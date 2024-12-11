#include "RelayModule.h"

RelayModule::RelayModule(int pin)
{
  this->pin = pin;
  this->status = RelayStatus::on;
}

void RelayModule::initialize()
{
  pinMode(this->pin, OUTPUT);
}

void RelayModule::turnOn()
{
  digitalWrite(this->pin, LOW); // LOW means on
  this->status = RelayStatus::on;
}

void RelayModule::turnOff()
{
  digitalWrite(this->pin, HIGH); // HIGH means off
  this->status = RelayStatus::off;
}

RelayStatus RelayModule::getStatus()
{
  return this->status;
}
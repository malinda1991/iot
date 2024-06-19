#include "Arduino.h"
#include "DoorSensor.h"
/**
* Class for magnetic door sensor switch
*
* @author Sandun Munasinghe
**/

DoorSensor::DoorSensor(byte doorMagnetPin, SensorType sensorType = SensorType::PRIMARY){
  this->pin = doorMagnetPin;
  this->doorState = LOW;
  if(sensorType == SensorType::SECONDARY){
    this->type = SensorType::SECONDARY;
  }else{
    this->type = SensorType::PRIMARY;
  }
}

void DoorSensor::initialize(){
  pinMode(this->pin, INPUT_PULLUP);
}

/**
* Reads the magentic door sensor current state and returns
* 
* @author Sandun Munasinghe
**/
GarageDoor DoorSensor::getDoorState(){
  this->doorState = digitalRead(this->pin);
  if(this->doorState == HIGH){
    return GarageDoor::OPEN;
  }else{
    return GarageDoor::CLOSED;
  }
}

static void DoorSensor::validateDevices(){
  DeviceError error;
  if(DoorSensor::primary.getDoorState() != DoorSensor::secondary.getDoorState()){
    // primary sensor seems faulty
    error.hasError = true;
  }else{
    // primary is working OK
    error.hasError = false;
    error.errorMessage = "OK";
  }
  DoorSensor::setErrorInfo(error);
}
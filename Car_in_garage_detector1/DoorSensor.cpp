#include "Arduino.h"
#include "DoorSensor.h"
/**
* Class for magnetic door sensor switch
*
* @author Sandun Munasinghe
**/

DoorSensor::DoorSensor(byte doorMagnetPin){
  this->pin = doorMagnetPin;
  this->doorState = LOW;
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
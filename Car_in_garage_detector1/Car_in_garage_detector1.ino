#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "CommonParams.h"
#include "DoorSensor.h"
#include "DhtSensor.h"
#include "NRF24L01Transceiver.h"
#include "UltrasonicSensor.h"
#include "AirQualitySensor.h"
#include "RelayModule.h"

const int NRF_CE_PIN = 7;
const int NRF_CSN_PIN = 8;
const int US_TRIG_PIN = 9;
const int US_ECHO_PIN = 10;
const int DOOR_MAG_PIN = 3;
const int LIGHTS_RELAY_PIN = 4;
const int DHT_PIN = 5;
const int MQ_AIR_QUALITY_PIN = A2;

const int carDistanceCm = 30;
const int LOOP_DELAY = 2000;

DoorSensor garageDoor = DoorSensor(DOOR_MAG_PIN);
DhtSensor garageDhtSensor = DhtSensor(DHT_PIN);
NRF24L01Transceiver radioTransmitter = NRF24L01Transceiver(
  NRF_CE_PIN,
  NRF_CSN_PIN,
  GARAGE_RF_CHANNEL,
  NULL,
  true,
  RF24_PA_MIN);
UltrasonicSensor usSensor = UltrasonicSensor(US_TRIG_PIN, US_ECHO_PIN);
AirQualitySensor aqSensor = AirQualitySensor(MQ_AIR_QUALITY_PIN);
RelayModule lightsRelay = RelayModule(LIGHTS_RELAY_PIN);

const String SEPERATOR = "!";
const int reservedBytes = 2;  // for the msg index values and seperators
const int msgBodyBytes = nrfPayloadBytesLimit - reservedBytes;


struct GarageData {
  float distance;
  CarInGarage isCarInGarage;
  GarageDoor door;
  int temperature;
  int humidity;
  GarageLights lights;
  int airQuality;
};

void operateGarageLights(GarageLights lights, GarageData *data) {
  if (lights == ON) {
    // Turn On the 230V relay
    lightsRelay.turnOn();
    data->lights = ON;
  } else {
    // Turn Off the 230V relay
    lightsRelay.turnOff();
    data->lights = OFF;
  }
}

/**
* Converts the data object to a json string and 
* sends it over radio to the receiver board
*
* @author Sandun Munasinghe
**/
void broadcastData(GarageData g1) {
  for (int x = 1; x <= 6; x++) {
    // iterations the number of messages needs to be sent
    String key = "";
    String value = "";
    switch (x) {
      case 1:
        // Is car in the garage
        key = "car";
        value = g1.isCarInGarage == YES ? "Y" : "N";
        break;
      case 2:
        // Garage door
        key = "door";
        value = g1.door == OPEN ? "O" : "C";
        break;
      case 3:
        // Temperature
        key = "temp";
        value = g1.temperature;
        break;
      case 4:
        // Humidity
        key = "humid";
        value = g1.humidity;
        break;
      case 5:
        // lights
        key = "lights";
        value = g1.lights == ON ? "O" : "F";
        break;
      case 6:
        // air quality
        key = "AQ";
        value = g1.airQuality;
        break;
    }

    /**
    * Message anatomy "{Garage ID}{seperator}{Data Key}{seperator}{Data Value}"
    **/
    String message = GARAGE_ID + SEPERATOR + key + SEPERATOR + value;
    Serial.println(message);
    radioTransmitter.sendRadioMessage(message);
  }
}

void setup() {
  usSensor.initialize();
  garageDoor.initialize();
  garageDhtSensor.initialize();
  lightsRelay.initialize();
  aqSensor.initialize();
  Serial.begin(9600);
  radioTransmitter.initialize();
}

void loop() {
  garageDhtSensor.updateSensorData();
  usSensor.updateSensorData();
  aqSensor.updateSensorData();

  struct GarageData g1;
  g1.distance = usSensor.getDistance();
  g1.isCarInGarage = NO;
  g1.door = garageDoor.getDoorState();
  g1.temperature = garageDhtSensor.getTemperature();
  g1.humidity = garageDhtSensor.getHumidity();
  g1.airQuality = aqSensor.getAirQualityLevel();

  if (g1.distance < carDistanceCm) {
    // car is in the garage
    g1.isCarInGarage = YES;
  }

  if (g1.door == OPEN) {
    // // Turn On the lights
    operateGarageLights(ON, &g1);
  } else {
    // // Turn Off the lights
    operateGarageLights(OFF, &g1);
  }

  broadcastData(g1);
  delay(LOOP_DELAY);
}

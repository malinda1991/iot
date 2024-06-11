#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "CommonParams.h"
#include "DoorSensor.h"
#include "DhtSensor.h"

const int US_TRIG_PIN = 9;
const int US_ECHO_PIN = 10;
const int DOOR_MAG_PIN = 3;
const int LIGHTS_RELAY_PIN = 4;
const int DHT_PIN = 5;
const int MQ_AIR_QUALITY_PIN = A2;
// const int lightButtonPin = 6;

const int carDistanceCm = 30;
const int LOOP_DELAY = 2000;

float duration, distance;
int doorState, lightButtonState, aqSensorData;
// bool garageLights = false;

DoorSensor garageDoor = DoorSensor(DOOR_MAG_PIN);
DhtSensor garageDhtSensor = DhtSensor(DHT_PIN);

RF24 radio(7, 8);  // CE, CSN

// const byte GARAGE_RF_CHANNEL[6] = "G1083";
const int rfPayloadBytesLimit = 32;  // NRF byte limit is 32
const String SEPERATOR = "!";
const int reservedBytes = 2;  // for the msg index values and seperators
const int msgBodyBytes = rfPayloadBytesLimit - reservedBytes;

// const String GARAGE_ID = "G1";

// ------------------ should put in common----------
const String ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


struct GarageData {
  int distance;
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
    digitalWrite(LIGHTS_RELAY_PIN, LOW);  // LOW means on
    data->lights = ON;
  } else {
    // Turn Off the 230V relay
    digitalWrite(LIGHTS_RELAY_PIN, HIGH);
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

  char message[rfPayloadBytesLimit] = "";
  message[0] = g1.isCarInGarage == YES ? "Y" : "N";
  message[1] = ",";
  message[2] = g1.door == OPEN ? "O" : "C";

  Serial.println(message);

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
    sendRadioMessage(message);
  }
}

void sendRadioMessage(String message) {
  char radioMessage[rfPayloadBytesLimit] = "";
  for (int x = 0; x < message.length(); x++) {
    radioMessage[x] = message[x];
  }
  radio.write(&radioMessage, sizeof(radioMessage));
  Serial.println(radioMessage);
  Serial.println("Radio message sent!");
}

void setup() {
  // put your setup code here, to run once:
  pinMode(US_TRIG_PIN, OUTPUT);
  pinMode(US_ECHO_PIN, INPUT);
  garageDoor.initialize();
  garageDhtSensor.initialize();
  pinMode(LIGHTS_RELAY_PIN, OUTPUT);
  pinMode(MQ_AIR_QUALITY_PIN, INPUT);
  // pinMode(lightButtonPin, INPUT_PULLUP);
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(GARAGE_RF_CHANNEL);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(US_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(US_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_TRIG_PIN, LOW);
  duration = pulseIn(US_ECHO_PIN, HIGH);
  distance = (duration * .0343) / 2;
  Serial.print("Distance: ");
  Serial.println(distance);

  // lightButtonState = digitalRead(lightButtonPin);

  garageDhtSensor.updateSensorData();
  aqSensorData = analogRead(MQ_AIR_QUALITY_PIN);

  struct GarageData g1;
  g1.distance = (int)distance;
  g1.isCarInGarage = NO;
  g1.door = garageDoor.getDoorState();
  g1.temperature = garageDhtSensor.getTemperature();
  g1.humidity = garageDhtSensor.getHumidity();
  g1.airQuality = aqSensorData;

  if (distance < carDistanceCm) {
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

  // if (lightButtonState == HIGH) {
  //   // light button pressed
  //   operateGarageLights(ON, &g1);
  // } else {
  //   // turn LED off:
  //   operateGarageLights(OFF, &g1);
  // }


  broadcastData(g1);
  delay(LOOP_DELAY);
}

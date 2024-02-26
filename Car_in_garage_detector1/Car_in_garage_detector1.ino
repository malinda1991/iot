#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Arduino_JSON.h>
#include <math.h>

const int trigPin = 9;
const int echoPin = 10;
const int carDistanceCm = 30;
const int LOOP_DELAY = 5000;
// const byte GARAGE_RF_CHANNEL[6] = "G1083";

float duration, distance;
RF24 radio(7, 8);  // CE, CSN

const byte address[6] = "G1083";
const int rfPayloadBytesLimit = 32;

/**
* Converts the data object to a json string and 
* sends it over radio to the receiver board
*
* @author Sandun Munasinghe
**/
void broadcastMessage(JSONVar dataObj) {
  String jsonPayload = JSON.stringify(dataObj);
  char message[jsonPayload.length()];

  Serial.print("json length = ");
  Serial.println(jsonPayload.length());

  for (int x = 0; x < jsonPayload.length(); x++) {
    message[x] = jsonPayload[x];
  }

  radio.write(&message, sizeof(message));
  Serial.println(message);
  Serial.println("Radio message sent!");
}

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * .0343) / 2;
  Serial.print("Distance: ");
  Serial.println(distance);

  String message;
  bool isCarInGarage = false;

  if (distance < carDistanceCm) {
    // car is in the garage
    message = "Car is in the garage :GREEN";
    isCarInGarage = true;
  } else {
    // car is not in the garage
    message = "Car is not in the garage :RED";
  }

  JSONVar dataObj;

  dataObj["dist"] = (int)distance;
  dataObj["car"] = isCarInGarage;
  dataObj["door"] = true;

  broadcastMessage(dataObj);
  Serial.println(message);
  Serial.println("-------------");
  delay(LOOP_DELAY);
}

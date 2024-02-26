#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <stdlib.h>
// #include <math.h>
#include <Arduino_JSON.h>

const int trigPin = 9;
const int echoPin = 10;
const int carDistanceCm = 30;
const int LOOP_DELAY = 5000;
// const byte GARAGE_RF_CHANNEL[6] = "G1083";

float duration, distance;
RF24 radio(7, 8);  // CE, CSN

const byte address[6] = "G1083";
const int rfPayloadBytesLimit = 32;
const char SEPERATOR = "~";
const int reservedBytes = 2;  // for the msgID and 1st seperator
const int msgBodyBytes = rfPayloadBytesLimit - reservedBytes;

char lastSentMsgId = "";  // to avoid sending the same msgID as the last one

// ------------------ should put in common----------
const String ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
enum CarInGarage {
  NO,
  YES
};

enum GarageDoor {
  OPEN,
  CLOSED
};

struct garageData {
  int distance;
  CarInGarage isCarInGarage;
  GarageDoor door;
  String extra;
};

// ------------------ end of should put in common----------
String buildJsonBody(garageData g1) {
  JSONVar dataObj;

  dataObj["dist"] = g1.distance;
  dataObj["car"] = g1.isCarInGarage;
  dataObj["door"] = g1.door;

  return JSON.stringify(dataObj);
}

char getMsgId() {
  char msgId = ALPHABET[random() % 26];
  while (msgId == lastSentMsgId) {
    msgId = ALPHABET[random() % 26];
  }
  lastSentMsgId = msgId;
  return msgId;
}

void breakMessageToChunks(garageData g1) {
  char msgId = getMsgId();
  String msgJsonBody = buildJsonBody(g1);
  
}

/**
* Converts the data object to a json string and 
* sends it over radio to the receiver board
*
* @author Sandun Munasinghe
**/
void broadcastMessage(garageData g1) {
  breakMessageToChunks(g1);
  // char message[jsonPayload.length()];

  // Serial.print("json length = ");
  // Serial.println(jsonPayload.length());

  // for (int x = 0; x < jsonPayload.length(); x++) {
  //   message[x] = jsonPayload[x];
  // }

  // radio.write(&message, sizeof(message));
  // Serial.println(msgChunks);
  // Serial.println("Radio message sent!");
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
  CarInGarage isCarInGarage = NO;

  if (distance < carDistanceCm) {
    // car is in the garage
    message = "Car is in the garage :GREEN";
    isCarInGarage = YES;
  } else {
    // car is not in the garage
    message = "Car is not in the garage :RED";
  }

  struct garageData g1;
  g1.distance = (int)distance;
  g1.isCarInGarage = isCarInGarage;
  g1.door = CLOSED;
  g1.extra = "abcdefghijklmopqrstuvwxyz";


  broadcastMessage(g1);
  Serial.println(message);
  Serial.println("-------------");
  delay(LOOP_DELAY);
}

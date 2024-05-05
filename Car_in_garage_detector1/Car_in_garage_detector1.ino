#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int trigPin = 9;
const int echoPin = 10;
const int doorSensorPin = 3;
const int relayPin = 4;
const int carDistanceCm = 30;
const int LOOP_DELAY = 2000;

float duration, distance;
int doorState;

RF24 radio(7, 8);  // CE, CSN

const byte GARAGE_RF_CHANNEL[6] = "G1083";
const int rfPayloadBytesLimit = 32;  // NRF byte limit is 32
const char SEPERATOR = "_";
const int reservedBytes = 2;  // for the msg index values and seperators
const int msgBodyBytes = rfPayloadBytesLimit - reservedBytes;

const String GARAGE_ID = "G1";

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

enum GarageLights {
  ON,
  OFF
};

struct garageData {
  int distance;
  CarInGarage isCarInGarage;
  GarageDoor door;
  int temperature;
  int humidity;
  GarageLights lights;
};

/**
* Converts the data object to a json string and 
* sends it over radio to the receiver board
*
* @author Sandun Munasinghe
**/
void broadcastData(garageData g1) {

  char message[rfPayloadBytesLimit] = "";
  message[0] = g1.isCarInGarage == YES ? "Y" : "N";
  message[1] = ",";
  message[2] = g1.door == OPEN ? "O" : "C";

  Serial.println(message);

  for(int x = 1; x<=5; x++){
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
    }

/**
* Message anatomy "{Garage ID}{seperator}{Data Key}{seperator}{Data Value}"
**/
      String message = GARAGE_ID + SEPERATOR + key + SEPERATOR + value;
      Serial.println(message);
      sendRadioMessage(message);
      // delay(100);
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
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(doorSensorPin, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(GARAGE_RF_CHANNEL);
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

  doorState = digitalRead(doorSensorPin);

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

  Serial.println("Door state ");
  Serial.print(doorState);

  struct garageData g1;
  g1.distance = (int)distance;
  g1.isCarInGarage = isCarInGarage;
  g1.door = doorState == HIGH ? OPEN : CLOSED;
  g1.temperature = 40;
  g1.humidity = 20;

  // 230V relay trigger
  if(g1.door == OPEN){
    // Turn On the relay 
    digitalWrite(relayPin, LOW); // LOW means on
    g1.lights = ON;
  }else{
    // Turn Off the relay
    digitalWrite(relayPin, HIGH);
    g1.lights = OFF;
  }


  broadcastData(g1);
  delay(LOOP_DELAY);
}

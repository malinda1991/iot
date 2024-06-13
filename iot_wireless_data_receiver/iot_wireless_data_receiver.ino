#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte GARAGE_RF_CHANNEL[6] = "G1083";
// The number of bytes that be sent through NRF24 at one time to one direction
const int nrfPayloadLimit = 32;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, GARAGE_RF_CHANNEL);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (radio.available()) {
    Serial.println("Receiving data");
    char text[nrfPayloadLimit] = "";
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
}

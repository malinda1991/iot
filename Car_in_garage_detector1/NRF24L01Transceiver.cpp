#include "NRF24L01Transceiver.h"

NRF24L01Transceiver::NRF24L01Transceiver(
  int cePin,
  int csnPin,
  byte* writePipeAddress = NULL,
  byte readPipeAddresses = NULL,
  boolean isTransmitterOnly = false,
  rf24_pa_dbm_e transceiverPowerLevel = RF24_PA_MIN) {
  this->cePin = cePin;
  this->csnPin = csnPin;
  this->writePipeAddress = writePipeAddress;
  this->readPipeAddresses = readPipeAddresses;
  this->isTransmitterOnly = isTransmitterOnly;
  this->transceiverPowerLevel = transceiverPowerLevel;
}


void NRF24L01Transceiver::initialize() {
  Serial.println("Radio begin");
  this->radio = RF24(this->cePin, this->csnPin);
  this->radio.begin();

  if (this->writePipeAddress) {
    this->radio.openWritingPipe(this->writePipeAddress);
  }

  if (this->readPipeAddresses) {
    this->radio.openReadingPipe(0, this->readPipeAddresses);
  }

  this->radio.setPALevel(this->transceiverPowerLevel);

  if (this->isTransmitterOnly) {
    Serial.println("Stop listening");
    this->radio.stopListening();
  } else {
    Serial.println("Start listening");
    this->radio.startListening();
  }
}

String NRF24L01Transceiver::receive() {
  if (this->radio.available()) {
    char text[nrfPayloadBytesLimit] = "";
    this->radio.read(&text, sizeof(text));
    return text;
  }
  return "";
}

void NRF24L01Transceiver::sendRadioMessage(String message) {
  char radioMessage[nrfPayloadBytesLimit] = "";
  for (int x = 0; x < message.length(); x++) {
    radioMessage[x] = message[x];
  }
  this->radio.write(&radioMessage, sizeof(radioMessage));
  Serial.println("Radio message sent!");
}
#include "WString.h"
#ifndef NRF24L01_TRANSCEIVER_H
#define NRF24L01_TRANSCEIVER_H

#include <Arduino.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "CommonParams.h"

const int nrfPayloadBytesLimit = 32;

class NRF24L01Transceiver {
private:
  int cePin;
  int csnPin;
  byte* writePipeAddress;
  byte readPipeAddresses;
  RF24 radio;
  boolean isTransmitterOnly;
  rf24_pa_dbm_e transceiverPowerLevel;

public:
  NRF24L01Transceiver(
    int cePin,
    int csnPin,
    byte* writePipeAddress,
    byte readPipeAddresses,
    boolean isTransmitterOnly,
    rf24_pa_dbm_e transceiverPowerLevel);
  void initialize();
  void transmit(String message);
  String receive();
  void sendRadioMessage(String message);
};

#endif
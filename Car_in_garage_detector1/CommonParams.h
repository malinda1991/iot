#ifndef COMMON_PARAMS_H
#define COMMON_PARAMS_H

enum GarageDoor {
  OPEN,
  CLOSED
};

enum GarageLights {
  ON,
  OFF
};

const String ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const byte GARAGE_RF_CHANNEL[6] = "G1083";
const String GARAGE_ID = "G1";

#endif
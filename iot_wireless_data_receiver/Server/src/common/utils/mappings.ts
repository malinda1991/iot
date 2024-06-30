/**
 * Common utility to manage key value mappings
 */

type ReceiverDataKeys = {
  car: string;
  garageDoor: string;
  temperature: string;
  humidity: string;
  garageLights: string;
  airQuality: string;
  carDistance: string;
  batteryLevel: string;
  deviceOnlineState: string;
};
type KeyMapper = Record<string, string>;
type ValueMapper = Record<string, KeyMapper>;

const flagsToAccept = ['G1'];
const seperator = '!';

const values = {
  INSIDE: 'Inside',
  OUTSIDE: 'Outside',
  OPEN: 'Open',
  CLOSED: 'Closed',
  ON: 'ON',
  OFF: 'OFF',
};

const keys: ReceiverDataKeys = {
  car: 'car',
  garageDoor: 'door',
  temperature: 'temp',
  humidity: 'humid',
  garageLights: 'lights',
  airQuality: 'AQ',
  carDistance: 'carDist',
  batteryLevel: 'btty',
  deviceOnlineState: 'online',
};

const keyMapping: KeyMapper = {
  [keys.car]: 'Car',
  [keys.garageDoor]: 'Door',
  [keys.temperature]: 'Temperature',
  [keys.humidity]: 'Humidity',
  [keys.garageLights]: 'Garage lights',
  [keys.airQuality]: 'Air Quality',
  [keys.carDistance]: 'Car Distance',
  [keys.batteryLevel]: 'Battery Level',
  [keys.deviceOnlineState]: 'Device Online State',
};
const valueMapping: ValueMapper = {
  [keys.car]: {
    Y: values.INSIDE,
    N: values.OUTSIDE,
  },
  [keys.garageDoor]: {
    O: values.OPEN,
    C: values.CLOSED,
  },
  [keys.garageLights]: {
    O: values.ON,
    F: values.OFF,
  },
};

const getGarageKeyName = (extractedData: { key: string; value: string }): string => {
  return keyMapping[extractedData.key];
};
const getGarageValueName = (extractedData: { key: string; value: string }): string => {
  const { key, value } = extractedData;
  if (valueMapping[key] !== undefined) {
    // has a value bind
    return valueMapping[key][value];
  } else {
    // return the value as it is
    return value;
  }
};

export const garageData = {
  keys,
  values,
  flagsToAccept,
  seperator,
  getGarageKeyName,
  getGarageValueName,
};

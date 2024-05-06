/**
 * Common utility to manage key value mappings
 */

type KeyMapper = Record<string, string>;
type ValueMapper = Record<string, KeyMapper>;

const flagsToAccept = ['G1'];
const seperator = '!';

const keyMapping: KeyMapper = {
  car: 'Car',
  door: 'Door',
  temp: 'Temperature',
  humid: 'Humidity',
  lights: 'Garage lights',
};
const valueMapping: ValueMapper = {
  car: {
    Y: 'Inside',
    N: 'Outside',
  },
  door: {
    O: 'Open',
    C: 'Closed',
  },
  lights: {
    O: 'ON',
    F: 'OFF',
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
  flagsToAccept,
  seperator,
  getGarageKeyName,
  getGarageValueName,
};

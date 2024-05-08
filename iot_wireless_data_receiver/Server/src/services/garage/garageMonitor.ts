import homeAssistant from '@/common/libraries/homeAssistant';
import { getFromMemoryCache, putToMemoryCache } from '@/common/libraries/memoryCache';
import serialPort from '@/common/libraries/serialPort';
import { garageData } from '@/common/utils/mappings';

const garageDataSenderParams: {
  intervalRef: NodeJS.Timeout | null;
  intervalPeriod: number;
} = {
  intervalRef: null,
  intervalPeriod: 20000,
};

const {
  keys: garageKeys,
  // values: garageValues,
  flagsToAccept,
  seperator,
  getGarageKeyName,
  getGarageValueName,
} = garageData;

const onStateChange = {
  garageDoor: (garageDoorState: string) => {
    // runs on garage door state change
    const notificationText = `The garage door is ${garageDoorState}`;
    homeAssistant.callNotificationsWebhook({
      title: 'Garage Door : ',
      message: notificationText,
    });
  },
  car: (carState: string) => {
    // runs on car state change

    const notificationText = `The car is ${carState}`;
    homeAssistant.callNotificationsWebhook({
      title: 'Car : ',
      message: notificationText,
    });
  },
  garageLights: (lightsState: string) => {
    // runs on garage lights state change
    const notificationText = `The garage lights are ${lightsState}`;
    homeAssistant.callNotificationsWebhook({
      title: 'Garage Lights : ',
      message: notificationText,
    });
  },
};

const checkForStateChanges = (dataObjFromCache: any, key: string, translatedValue: string) => {
  const valueChangeCheck = (triggerCallback: any) => {
    if (dataObjFromCache[key]?.value !== undefined && dataObjFromCache[key]?.value !== translatedValue) {
      // the state has been changed and it is not undefined
      triggerCallback(translatedValue);
    }
  };

  switch (key) {
    case garageKeys.garageDoor:
      valueChangeCheck(onStateChange.garageDoor);
      break;
    case garageKeys.car:
      valueChangeCheck(onStateChange.car);
      break;
    case garageKeys.garageLights:
      valueChangeCheck(onStateChange.garageLights);
      break;
    default:
      break;
  }
};

const invokeMonitor = async () => {
  serialPort.runScanner((data: string) => {
    flagsToAccept.forEach((flag) => {
      if (data.includes(flag)) {
        // data string should be accepted
        const seperatedDataString = data.split(seperator);

        if (seperatedDataString[1] && seperatedDataString[2]) {
          // both elements exists
          const extractedData = {
            key: seperatedDataString[1],
            value: seperatedDataString[2].replace('\r', ''),
          };

          const dataObjFromCache = getFromMemoryCache(flag) || {};

          const translatedData = {
            id: flag,
            [extractedData.key]: {
              name: getGarageKeyName(extractedData),
              value: getGarageValueName(extractedData),
            },
          };

          checkForStateChanges(dataObjFromCache, extractedData.key, translatedData[extractedData.key].value);

          putToMemoryCache(flag, {
            ...dataObjFromCache,
            ...translatedData,
          });
        }
      }
    });
  });
};

const invokeGarageDataSender = async () => {
  console.log('invoked GarageDataSender');
  const callback = () => {
    const promises = flagsToAccept.map(async (flag) => {
      const dataObjFromCache = getFromMemoryCache(flag) || null;
      if (dataObjFromCache) {
        return homeAssistant.callSaveDataWebhook(dataObjFromCache);
      }
    });

    Promise.all(promises);
  };

  garageDataSenderParams.intervalRef = setInterval(callback, garageDataSenderParams.intervalPeriod);
};

export default {
  invokeMonitor,
  invokeGarageDataSender,
};

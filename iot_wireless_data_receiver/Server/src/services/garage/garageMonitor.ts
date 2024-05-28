import homeAssistant, { SaveDataWebhookRequest } from '@/common/libraries/homeAssistant';
import { getFromMemoryCache, putToMemoryCache, CACHE_ID } from '@/common/libraries/memoryCache';
import serialPort from '@/common/libraries/serialPort';
import { garageData } from '@/common/utils/mappings';
import { translate } from '@/common/libraries/i18n';

const garageDataSenderParams: {
  intervalRef: NodeJS.Timeout | null;
  intervalPeriod: number;
} = {
  intervalRef: null,
  intervalPeriod: 20000,
};

const radioEntityTrackerParams: {
  intervalRef: NodeJS.Timeout | null;
  intervalPeriod: number;
} = {
  intervalRef: null,
  intervalPeriod: 3000,
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
  garageDoor: (garageDoorState: string, data: SaveDataWebhookRequest) => {
    // runs on garage door state change
    const notificationText = `The garage door is ${garageDoorState}`;
    homeAssistant.callNotificationsWebhook({
      title: 'Garage Door : ',
      message: notificationText,
      data,
    });
  },
  car: (carState: string, data: SaveDataWebhookRequest) => {
    // runs on car state change
    const notificationText = `The car is ${carState}`;
    homeAssistant.callNotificationsWebhook({
      title: 'Car : ',
      message: notificationText,
      data,
    });
  },
  garageLights: (lightsState: string, data: SaveDataWebhookRequest) => {
    // runs on garage lights state change
    const notificationText = `The garage lights are ${lightsState}`;
    homeAssistant.callNotificationsWebhook({
      title: 'Garage Lights : ',
      message: notificationText,
      data,
    });
  },
};

const checkForStateChanges = (dataObjFromCache: any, key: string, translatedDataObj: any) => {
  const translatedValue = translatedDataObj[key].value;
  const valueChangeCheck = (triggerCallback: any) => {
    if (dataObjFromCache[key]?.value !== undefined && dataObjFromCache[key]?.value !== translatedValue) {
      // the state has been changed and it is not undefined
      triggerCallback(translatedValue, translatedDataObj);
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
            ...dataObjFromCache,
            [extractedData.key]: {
              name: getGarageKeyName(extractedData),
              value: getGarageValueName(extractedData),
            },
          };

          checkForStateChanges(dataObjFromCache, extractedData.key, translatedData);

          putToMemoryCache(flag, translatedData);
        }
      }

      recordLastPayloadReceivedEvent(flag);
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

const recordLastPayloadReceivedEvent = (entityId: string) => {
  const dataObjFromCache = getFromMemoryCache(CACHE_ID.lastRecordedTime) || {};
  dataObjFromCache[entityId] = Date.now();
  putToMemoryCache(CACHE_ID.lastRecordedTime, dataObjFromCache);
};

const invokeRadioEntityAvailabilityCheck = () => {
  console.log('invoked Radio entity availabilty check');
  const alertThresholdMilliSeconds: number = 5000;
  const callback = () => {
    const dataObjFromCache = getFromMemoryCache(CACHE_ID.lastRecordedTime) || null;
    if (dataObjFromCache) {
      const currentTimeInMilliSeconds = Date.now();
      const entities = Object.keys(dataObjFromCache);
      entities.forEach((entityId: string) => {
        const lastRecordedTime = dataObjFromCache[entityId];
        if (lastRecordedTime) {
          // has a last recorded time in the memory
          if (currentTimeInMilliSeconds - lastRecordedTime > alertThresholdMilliSeconds) {
            // has not received data than the alert threshold time
            console.log(`*****${entityId} is not available`);
          }
        }
      });
    }
  };

  radioEntityTrackerParams.intervalRef = setInterval(callback, radioEntityTrackerParams.intervalPeriod);
};

export default {
  invokeMonitor,
  invokeGarageDataSender,
  invokeRadioEntityAvailabilityCheck,
};

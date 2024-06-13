import homeAssistant, { SaveDataWebhookRequest } from '@/common/libraries/homeAssistant';
import { getFromMemoryCache, putToMemoryCache, CACHE_ID } from '@/common/libraries/memoryCache';
import serialPort from '@/common/libraries/serialPort';
import { garageData } from '@/common/utils/mappings';
import { translate } from '@/common/libraries/i18n';

enum ThresholdDirection {
  LOWER,
  HIGHER
}

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

const { CAR_DISTANCE_CM } = {
  CAR_DISTANCE_CM: 30,
};

const { keys: garageKeys, flagsToAccept, seperator, getGarageKeyName, getGarageValueName } = garageData;

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
  temperature: (temperatureState: string, data: SaveDataWebhookRequest) => {
    // runs on garage lights state change
    const notificationText = `The garage temperature is ${temperatureState}`;
    homeAssistant.callNotificationsWebhook({
      title: 'Garage Temperature : ',
      message: notificationText,
      data,
    });
  },
  humidity: (humidityState: string, data: SaveDataWebhookRequest) => {
    // runs on garage lights state change
    const notificationText = `The garage humidity is ${humidityState}`;
    homeAssistant.callNotificationsWebhook({
      title: 'Garage Humidity : ',
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
  const valueThresholdCheck = (triggerCallback: any, thresholdLimit : number = 0, thresholdDirection : ThresholdDirection = ThresholdDirection.HIGHER) => {
    const previousValue = dataObjFromCache[key]?.value || 0;
    if(thresholdDirection == ThresholdDirection.LOWER){
      // logic if went same or below the threshold
      if(parseFloat(translatedValue) <= thresholdLimit && parseFloat(previousValue) > thresholdLimit){

        triggerCallback(`low (${translatedValue})`, translatedDataObj);
      }
    }else{
      // logic if went same or above the threshold
      if(parseFloat(translatedValue) >= thresholdLimit && parseFloat(previousValue) < thresholdLimit){

        console.log('------------------------translatedDataObj', translatedDataObj);
        triggerCallback(`high (${translatedValue})`, translatedDataObj);
      }
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
    case garageKeys.temperature:
      valueThresholdCheck(onStateChange.temperature, 20, ThresholdDirection.LOWER);
      break;
    case garageKeys.humidity:
      valueThresholdCheck(onStateChange.humidity, 80, ThresholdDirection.HIGHER);
      break;
    default:
      break;
  }
};

const translateReceivedData = (flag: string, extractedData: { key: string; value: string }) => {
  const dataObjFromCache = getFromMemoryCache(flag) || {};

  const translatedData = {
    id: flag,
    ...dataObjFromCache,
    [extractedData.key]: {
      name: getGarageKeyName(extractedData),
      value: getGarageValueName(extractedData),
    },
  };

  let extractedKey = extractedData.key;

  switch (extractedData.key) {
    case garageKeys.carDistance: {
      // detect car availability
      const carData = {
        key: garageKeys.car,
        value: 'N',
      };

      if (parseFloat(extractedData.value) < CAR_DISTANCE_CM) {
        // car is in the garage
        carData.value = 'Y';
      }

      translatedData[garageKeys.car] = {
        name: getGarageKeyName(carData),
        value: getGarageValueName(carData),
      };
      extractedKey = garageKeys.car;
      break;
    }

    default:
      break;
  }

  return {
    dataObjFromCache,
    translatedData,
    extractedKey
  };
};

const invokeMonitor = async () => {
  serialPort.runScanner((data: string) => {
    console.log('Scanning..', data);
    flagsToAccept.forEach((flag) => {
      if (data.includes(flag)) {
        // data string should be accepted
        console.log(`${flag} data received`);
        const seperatedDataString = data.split(seperator);

        if (seperatedDataString[1] && seperatedDataString[2]) {
          // both elements exists
          const extractedData = {
            key: seperatedDataString[1],
            value: seperatedDataString[2].replace('\r', ''),
          };

          const { translatedData, dataObjFromCache, extractedKey } = translateReceivedData(flag, extractedData);

          checkForStateChanges(dataObjFromCache, extractedKey, translatedData);

          putToMemoryCache(flag, translatedData);
        }

        recordLastPayloadReceivedEvent(flag);
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

// import { isMainThread, parentPort, Worker } from 'worker_threads';

import { putToMemoryCache } from '@/common/libraries/memoryCache';
import serialPort from '@/common/libraries/serialPort';
import { garageData } from '@/common/utils/mappings';

const { flagsToAccept, seperator, getGarageKeyName, getGarageValueName } = garageData;

const invokeWorker = () => {
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

          const translatedData = {
            id: flag,
            key: getGarageKeyName(extractedData),
            value: getGarageValueName(extractedData),
          };

          putToMemoryCache(translatedData.key, translatedData.value);
          console.log(translatedData);
        }
      }
    });
  });

  //   if (isMainThread) {
  //     // This code is executed in the main thread and not in the worker.

  //     // Create the worker.
  //     const worker = new Worker(__filename);
  //     // Listen for messages from the worker and print them.
  //     worker.on('message', (msg) => {
  //       console.log(msg);
  //     });
  //   } else {
  //     // This code is executed in the worker and not in the main thread.
  //     parentPort.postMessage('Called by worker');
  //   }
};

export default {
  invokeWorker,
};

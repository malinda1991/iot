/**
 * Library for the Serial port module
 *
 * @author Sandun Munasinghe
 * @since 5/5/2024
 */

import { ReadlineParser as Readline } from '@serialport/parser-readline';
import { SerialPort } from 'serialport';

import { env } from '@/common/utils/envConfig';

const { USB_PORT_ADDRESS, BAUD_RATE } = env.SERIAL_MONITOR;

const port = new SerialPort({
  path: USB_PORT_ADDRESS,
  baudRate: BAUD_RATE,
});
const parser = port.pipe(
  new Readline({
    delimiter: '\n',
  })
);

const runScanner = (dataCallback: any): void => {
  // Read the port data
  port.on('open', () => {
    console.log('Serial port open');
  });
  port.on('error', (error) => {
    console.log('Error on serial port', error);
  });
  parser.on('data', (data: string) => {
    dataCallback(data);
  });
};

export default {
  runScanner,
};

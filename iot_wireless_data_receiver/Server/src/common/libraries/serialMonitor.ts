import { ReadlineParser as Readline } from '@serialport/parser-readline';
import { SerialPort } from 'serialport';

const USB_PORT_ADDRESS = 'COM5';
const BAUD_RATE = 9600;

const port = new SerialPort({
  path: USB_PORT_ADDRESS,
  baudRate: BAUD_RATE,
});
const parser = port.pipe(
  new Readline({
    delimiter: '\n',
  })
);

const runScanner = (): void => {
  // Read the port data
  port.on('open', () => {
    console.log('serial port open');
  });
  parser.on('data', (data) => {
    console.log('got word from arduino:', data);
  });
};

export default {
  runScanner,
};

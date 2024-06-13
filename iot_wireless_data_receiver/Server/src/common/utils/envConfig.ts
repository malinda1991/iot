import dotenv from 'dotenv';
import { cleanEnv, host, num, port, str, testOnly, url } from 'envalid';

dotenv.config();

const baseEnv = cleanEnv(process.env, {
  NODE_ENV: str({ devDefault: testOnly('test'), choices: ['development', 'production', 'test'] }),
  HOST: host({ devDefault: testOnly('localhost') }),
  PORT: port({ devDefault: testOnly(3000) }),
  CORS_ORIGIN: str({ devDefault: testOnly('http://localhost:3000') }),
  COMMON_RATE_LIMIT_MAX_REQUESTS: num({ devDefault: testOnly(1000) }),
  COMMON_RATE_LIMIT_WINDOW_MS: num({ devDefault: testOnly(1000) }),
});

const serialMonitorEnv = cleanEnv(process.env, {
  BAUD_RATE: num({ devDefault: testOnly(9600) }),
  USB_PORT_ADDRESS: str({ devDefault: testOnly('COM5') }),
});

const homeAssistantEnv = cleanEnv(process.env, {
  HA_WEBHOOK_BASE_URL: url({ devDefault: testOnly('http://192.168.0.150:8123/api/webhook') }),
  HA_WEBHOOK_ID_NOTIFICATIONS: str({ devDefault: testOnly('test') }),
  HA_WEBHOOK_ID_GARAGE_DATA: str({ devDefault: testOnly('test') }),
});

export const env = {
  ...baseEnv,
  SERIAL_MONITOR: serialMonitorEnv,
  HOME_ASSISTANT: homeAssistantEnv,
};

/**
 * Library for home assistant service interactions
 *
 * @author Sandun Munasinghe
 * @since 8/5/2024
 */

import { sendPostRequest } from '@/common/libraries/httpRequester';
import { env } from '@/common/utils/envConfig';

const { HA_WEBHOOK_BASE_URL, HA_WEBHOOK_ID_NOTIFICATIONS, HA_WEBHOOK_ID_GARAGE_DATA } = env.HOME_ASSISTANT;

const callWebhook = async (hookId: string, payload: any) => {
  const hookUrl = `${HA_WEBHOOK_BASE_URL}/${hookId}`;
  return await sendPostRequest(hookUrl, payload);
};

const callSaveDataWebhook = async (dataset: any) => {
  console.log('calling save data webhook', dataset);
  return callWebhook(HA_WEBHOOK_ID_GARAGE_DATA, dataset);
};

const callNotificationsWebhook = async (dataset: any) => {
  console.log('calling save data webhook', dataset);
  return await callWebhook(HA_WEBHOOK_ID_NOTIFICATIONS, dataset);
};

export default {
  callSaveDataWebhook,
  callNotificationsWebhook,
};

/**
 * Library for home assistant service interactions
 *
 * @author Sandun Munasinghe
 * @since 8/5/2024
 */

import { sendPostRequest } from '@/common/libraries/httpRequester';
import { env } from '@/common/utils/envConfig';

const { HA_WEBHOOK_BASE_URL, HA_WEBHOOK_ID_NOTIFICATIONS, HA_WEBHOOK_ID_GARAGE_DATA } = env.HOME_ASSISTANT;

export type SaveDataWebhookRequest = Record<string, { name: string; value: string }>;
export type NotificationWebhookRequest = {
  title: string;
  message: string;
  data?: SaveDataWebhookRequest;
};

const callWebhook = async (hookId: string, payload: any) => {
  const hookUrl = `${HA_WEBHOOK_BASE_URL}/${hookId}`;
  return await sendPostRequest(hookUrl, payload);
};

const callSaveDataWebhook = async (dataset: SaveDataWebhookRequest) => {
  console.log('calling save data webhook', dataset);
  return callWebhook(HA_WEBHOOK_ID_GARAGE_DATA, dataset);
};

const callNotificationsWebhook = async (dataset: NotificationWebhookRequest) => {
  console.log('calling notification data webhook', dataset);
  if (dataset?.data !== undefined) {
    // has data values to be sent to home assistant
    await callSaveDataWebhook(dataset.data);
  }
  return await callWebhook(HA_WEBHOOK_ID_NOTIFICATIONS, dataset);
};

export default {
  callSaveDataWebhook,
  callNotificationsWebhook,
};

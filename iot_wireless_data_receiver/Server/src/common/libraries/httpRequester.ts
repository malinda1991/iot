/**
 * Library for sending HTTP request
 *
 * @author Sandun Munasinghe
 * @since 8/5/2024
 */
import axios, { AxiosError, AxiosResponse } from 'axios';

/**
 * Sends out a POST HTTP request
 * @param url
 * @param payload
 * @returns Promise the response payload of succes or null on error
 *
 * @author Sandun Munasinghe
 * @since 8/5/2024
 */
export const sendPostRequest = async (url: string, payload?: any): Promise<AxiosResponse | null> => {
  console.log('calling ', url);
  return await axios
    .post(url, payload)
    .then((response: AxiosResponse) => {
      console.log('Axios POST request response recieved');
      return response;
    })
    .catch((error: AxiosError) => {
      console.log('Axios POST request error', error);
      return null;
    });
};

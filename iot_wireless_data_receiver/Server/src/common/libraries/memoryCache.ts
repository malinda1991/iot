/**
 * Library for the memory cache module
 *
 * @author Sandun Munasinghe
 * @since 6/5/2024
 */

import memCache from 'memory-cache';

/**
 * Put data to be saved on memory
 * @param key id for the value
 * @param value the value to be saved
 */
export const putToMemoryCache = (key: string, value: any): void => {
  memCache.put(key, value);
};

/**
 * Returns value from the memory which matches to the key
 * @param key id for the value
 * @returns the value matches to the key
 */
export const getFromMemoryCache = (key: string): any => {
  return memCache.get(key);
};

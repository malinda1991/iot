import garageMonitor from '@/services/workers/garageMonitor';

/**
 * executes a set of functions on server startup
 *
 * @author Sandun Munasinghe
 * @since 6/5/2024
 */
export const startupOperations = () => {
  garageMonitor.invokeMonitor();
  garageMonitor.invokeGarageDataSender();
};

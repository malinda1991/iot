import { env } from '@/common/utils/envConfig';
import { app, logger } from '@/server';
import garageMonitor from '@/services/workers/garageMonitor';

/**
 * executes a set of functions on server startup
 *
 * @author Sandun Munasinghe
 * @since 6/5/2024
 */
const startupOperations = () => {
  garageMonitor.invokeWorker();
};

const server = app.listen(env.PORT, () => {
  const { NODE_ENV, HOST, PORT } = env;
  logger.info(`Server (${NODE_ENV}) running on port http://${HOST}:${PORT}`);

  startupOperations();
});

const onCloseSignal = () => {
  logger.info('sigint received, shutting down');
  server.close(() => {
    logger.info('server closed');
    process.exit();
  });
  setTimeout(() => process.exit(1), 10000).unref(); // Force shutdown after 10s
};

process.on('SIGINT', onCloseSignal);
process.on('SIGTERM', onCloseSignal);

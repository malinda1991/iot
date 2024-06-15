/* eslint-disable prettier/prettier */
import { OpenAPIRegistry } from '@asteasolutions/zod-to-openapi';
import express, { Request, Response, Router } from 'express';
import { StatusCodes } from 'http-status-codes';
import { z } from 'zod';

import { createApiResponse } from '@/api-docs/openAPIResponseBuilders';
import { ResponseStatus, ServiceResponse } from '@/common/models/serviceResponse';
import { handleServiceResponse } from '@/common/utils/httpHandlers';

export const energyMeterRegistry = new OpenAPIRegistry();

export const energyMeterRouter: Router = (() => {
  const router = express.Router();

  energyMeterRegistry.registerPath({
    method: 'post',
    path: '/energy-meter/record',
    tags: ['Energy Meter Record'],
    responses: createApiResponse(z.null(), 'Success'),
  });

  router.post('/record', (_req: Request, res: Response) => {
    
    const serviceResponse = new ServiceResponse(
      ResponseStatus.Success,
      'Energy reading recorded successfully',
      null,
      StatusCodes.OK
    );
    handleServiceResponse(serviceResponse, res);
  });

  return router;
})();

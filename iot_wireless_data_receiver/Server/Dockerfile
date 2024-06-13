FROM node:current-slim

# Create app directory
WORKDIR /usr/src/iot-server

# Copy package.json and package-lock.json
COPY package*.json ./

# Install app dependencies
RUN npm ci

# Bundle app source
COPY . .

# Build the TypeScript files
RUN npm run build

# set environment variables
ENV NODE_ENV="production"
ENV PORT="7000"

# Expose port 7000
EXPOSE 7000

# Start the app
CMD npm run start

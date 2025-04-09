# Use Node.js LTS version as base image
FROM node:18-alpine

# Set working directory
WORKDIR /app

# Copy package files
COPY package*.json ./

# Install dependencies
RUN npm install

# Copy the rest of the application
COPY . .

# Expose port (adjust if your app uses a different port)
EXPOSE 3002

# Start the application
CMD ["npm", "run", "dev"]
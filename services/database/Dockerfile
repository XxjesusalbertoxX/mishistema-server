FROM node:18-slim

WORKDIR /app

USER root

COPY package*.json ./

RUN npm install

COPY . .


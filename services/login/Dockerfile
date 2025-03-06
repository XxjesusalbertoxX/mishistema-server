FROM node:16-alpine

WORKDIR /app

USER root

COPY package*.json ./

RUN npm install

COPY . .

EXPOSE 3003

CMD ["npm", "run", "dev"]
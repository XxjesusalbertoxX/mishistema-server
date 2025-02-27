FROM node:16-alpine

WORKDIR /app

COPY package*.json ./

RUN npm cache clean --force

RUN npm config set registry https://registry.npmjs.org/

RUN npm install

COPY . .

EXPOSE 3003

CMD ["npm", "run", "dev"]
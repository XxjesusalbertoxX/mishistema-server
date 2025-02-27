import express from 'express';
import proxy from 'express-http-proxy';
import dotenv from 'dotenv';
dotenv.config();

const app = express();
const PORT = Number(process.env.PORT) || 8080;

app.use('/auth', proxy('http://auth:3000'));

app.listen(PORT, () => {
  console.log('API Gateway corriendo en el puerto 8080');
});

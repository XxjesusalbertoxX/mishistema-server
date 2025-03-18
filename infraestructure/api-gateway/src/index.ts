import express from 'express';
import proxy from 'express-http-proxy';
import dotenv from 'dotenv';
import cors from 'cors';

// Cargar variables de entorno desde el archivo .env
dotenv.config();

const app = express();
const APIGATEWAY_PORT = Number(process.env.APIGATEWAY_PORT) || 8080;
const AUTH_SERVICE_ENDPOINT = process.env.AUTH_SERVICE!;

// Habilitar CORS para permitir que el frontend Android acceda a esta API
const corsOptions = {
  origin: '*', // Cambia esta URL según la de tu app Android o frontend
  methods: ['GET', 'POST', 'PUT', 'DELETE'],
  allowedHeaders: ['Content-Type', 'Authorization'],
};

app.use(cors(corsOptions));

// Proxy para redirigir solicitudes al servicio de autenticación
app.use('/auth', proxy(AUTH_SERVICE_ENDPOINT));

// Iniciar el servidor
app.listen(APIGATEWAY_PORT, () => {
  console.log(`API Gateway corriendo en el puerto ${APIGATEWAY_PORT}`);
});

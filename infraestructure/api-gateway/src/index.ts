import express from 'express';
import proxy from 'express-http-proxy';
import dotenv from 'dotenv';
import cors from 'cors';

// Cargar variables de entorno desde el archivo .env
dotenv.config();

const app = express();
const PORT = Number(process.env.PORT) || 8080;
const API_IOT = process.env.API_IOT!;
const AUTH_SERVICE = process.env.AUTH_SERVICE!;

// Habilitar CORS para permitir que el frontend Android acceda a esta API
const corsOptions = {
  origin: 'http://18.118.45.70:8080', // Cambia esta URL según la de tu app Android o frontend
  methods: ['GET', 'POST', 'PUT', 'DELETE'],
  allowedHeaders: ['Content-Type', 'Authorization'],
};

app.use(cors(corsOptions));

// Proxy para redirigir solicitudes al servicio de autenticación
app.use('/auth', proxy(AUTH_SERVICE));

// Proxy para redirigir solicitudes al API de IoT
app.use('/iot', proxy(API_IOT));

// Iniciar el servidor
app.listen(PORT, () => {
  console.log(`API Gateway corriendo en el puerto ${PORT}`);
});

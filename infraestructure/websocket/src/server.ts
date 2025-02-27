import { Server } from 'socket.io';
import dotenv from "dotenv";

//middleware
dotenv.config();

const PORT = Number(process.env.PORT) || 3005;

const io = new Server(PORT, {
  cors: {
    origin: "*",
  },
});

io.on('connection', (socket) => {
  console.log('Nuevo cliente conectado');
  socket.on('disconnect', () => {
    console.log('Cliente desconectado');
  });
});

console.log('Servidor WebSocket corriendo en el puerto 3005');

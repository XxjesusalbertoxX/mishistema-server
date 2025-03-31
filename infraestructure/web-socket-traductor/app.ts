import * as mqtt from 'mqtt';
import * as WebSocket from 'ws';
import dotenv from 'dotenv';

dotenv.config();

const MQTT_BROKER = process.env.MQTT_BROKER; // Broker MQTT
const WS_PORT = process.env.WS_PORT; // Puerto para WebSocket

// Opciones de conexión MQTT con credenciales
const mqttOptions = {
    username: process.env.MQTT_USERNAME,
    password: process.env.MQTT_PASSWORD,
    connectTimeout: 5000,
    reconnectPeriod: 5000,
};


const mqttClient = mqtt.connect(MQTT_BROKER, mqttOptions); // Conectar a MQTT con autenticación
const wss = new WebSocket.Server({ port: WS_PORT }); // Crear servidor WebSocket

// Agregar manejo de errores MQTT
mqttClient.on('error', (err) => {
    console.error('Error de conexión MQTT:', err);
});

mqttClient.on('offline', () => {
    console.log('Cliente MQTT desconectado');
});

// Cuando un cliente WebSocket se conecta
wss.on('connection', (ws: WebSocket) => {
    console.log('Cliente WebSocket conectado');

    // Cuando el cliente WebSocket se desconecta
    ws.on('close', () => {
        console.log('Cliente WebSocket desconectado');
    });
});

// Cuando un mensaje llega de MQTT
mqttClient.on('connect', () => {
    console.log(`Conectado al broker MQTT en ${MQTT_BROKER}`);
    mqttClient.subscribe('#', (err) => { // Suscribirse a todos los topics con '#'
        if (err) {
            console.error('Error al suscribirse a todos los topics:', err);
        } else {
            console.log('Suscrito a todos los topics');
        }
    });
});

mqttClient.on('message', (topic: string, message: Buffer) => {
    const messageStr = message.toString(); // Convertir mensaje a string

    console.log(`Mensaje recibido de MQTT en ${topic}: ${messageStr}`);

    // Enviar el mensaje a todos los clientes WebSocket conectados
    wss.clients.forEach((client) => {
        if (client.readyState === WebSocket.OPEN) {
            const wsMessage = {
                topic: topic,
                message: messageStr
            };
            client.send(JSON.stringify(wsMessage));
        }
    });
});

console.log(`Servidor WebSocket corriendo en ws://localhost:${WS_PORT}`);
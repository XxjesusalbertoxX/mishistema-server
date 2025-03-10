import mqtt, { IClientSubscribeOptions } from 'mqtt';
import dotenv from 'dotenv';

dotenv.config();

const MQTT_BROKER_URL = process.env.RABBITMQ_MQTT_URL || 'mqtt://3.145.125.14:1883';
const TOPIC = 'pene';

const client = mqtt.connect(MQTT_BROKER_URL, {
  username: process.env.RABBITMQ_USER || 'admin',
  password: process.env.RABBITMQ_PASSWORD || 'admin123'
});

client.on('connect', () => {
  console.log('✅ Conectado a RabbitMQ MQTT');
  client.subscribe(TOPIC, (err: Error | null) => {
    if (err) {
      console.error('❌ Error al suscribirse al topic:', err);
    } else {
      console.log(`📡 Suscrito al topic: ${TOPIC}`);
    }
  });
});

client.on('message', (topic, message) => {
  console.log(`📥 Mensaje recibido en ${topic}:`, message.toString());
});

client.on('error', (err) => {
  console.error('❌ Error de conexión MQTT:', err);
});

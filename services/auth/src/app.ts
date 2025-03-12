import express from 'express';
import passport from 'passport';
import { login, authenticate } from './services/authService.js';
import mqtt from 'mqtt';

const mqttClient = mqtt.connect('mqtt://18.117.196.152:1883', {
  username: 'rabbit345', 
  password: 'rabbitseguro345' 
});

const app = express();
app.use(express.json());
app.use(passport.initialize());

app.post('/login', login);

mqttClient.on('connect', () => {
  console.log('Conectado a MQTT Broker');
  mqttClient.subscribe('pene', (err) => {
    if (!err) {
      console.log('Suscrito al topic sensorData');
    }
  });

 
  setInterval(() => {
    const message = JSON.stringify({ sensor: 'temperature', value: Math.random() * 100 });
    mqttClient.publish('sensorData', message);
    console.log('Mensaje enviado:', message);
  }, 5000); 
});

mqttClient.on('message', (topic, message) => {
  if (topic === 'pene') {
    console.log('Mensaje recibido:', message.toString());
  }
});

app.get('/protected', authenticate, (req, res) => {
  res.json({ message: 'This is a protected route' });
});

const PORT = process.env.PORT || 3001;
app.listen(PORT, () => {
  console.log(`Auth service running on port ${PORT}`);
});
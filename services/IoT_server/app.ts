import WebSocket from 'ws';

// URL del servidor WebSocket que está configurado para MQTT
const WS_URL = 'ws://3.148.105.137:8083/mqtt';

// Credenciales de autenticación (usuario y contraseña)
const username = 'sebas';
const password = 'sebas123';

// Crear un objeto WebSocket, pasando las credenciales de autenticación en las cabeceras HTTP
const socket = new WebSocket(WS_URL, {
    headers: {
        'Authorization': 'Basic ' + Buffer.from(`${username}:${password}`).toString('base64')
    }
});

socket.on('open', () => {
    console.log('Connected to WebSocket server');

    // Aquí debes enviar el mensaje para suscribirte al topic 'prueba'
    const subscribeMessage = JSON.stringify({
        action: 'subscribe',  // Define que es una acción de suscripción
        topic: 'prueba'       // El topic al que te quieres suscribir
    });

    // Envía el mensaje de suscripción al servidor WebSocket
    socket.send(subscribeMessage);
});

socket.on('message', (data) => {
    // Cuando recibas un mensaje, lo imprimes en consola
    console.log('Received message from topic "prueba":', data.toString());
});

socket.on('error', (error) => {
    console.error('WebSocket error:', error);
});

socket.on('close', (code, reason) => {
    console.log(`WebSocket closed: ${code} - ${reason}`);
});

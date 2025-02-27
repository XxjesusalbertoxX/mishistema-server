#include <WiFiNINA.h>  // Para el MKR WiFi 1010
#include <PubSubClient.h>  // Librería para MQTT
#include <Stepper.h>

// Configuraciones WiFi
const char *ssid = "TuRedWiFi";
const char *password = "TuContraseñaWiFi";

// Configuración MQTT
const char *mqttServer = "192.168.1.70"; // IP de tu servidor MQTT (RabbitMQ)
const int mqttPort = 1883;  // Puerto MQTT
const char *mqttUser = "admin";  // Usuario RabbitMQ
const char *mqttPassword = "admin123";  // Contraseña RabbitMQ
const char *mqttTopic = "motor/configure";  // El tópico al que se suscribe Arduino

WiFiClient espClient;
PubSubClient client(espClient);

// Configurar el motor paso a paso
#define STEPS_PER_REV 2048  // 512 pasos = 360° para el 28BYJ-48
Stepper stepper(STEPS_PER_REV, 4, 6, 5, 7);

// Variables para velocidad y pasos
int motorSpeed = 10;  // Velocidad inicial en RPM
int motorSteps = 512;  // Pasos iniciales (medio giro)

void setup() {
    Serial.begin(115200);  // Inicia la comunicación serie
    WiFi.begin(ssid, password);  // Conecta a Wi-Fi

    // Espera hasta estar conectado
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando a WiFi...");
    }
    Serial.println("Conectado a WiFi");

    // Configura el cliente MQTT
    client.setServer(mqttServer, mqttPort);
    client.setCallback(mqttCallback);

    stepper.setSpeed(motorSpeed);  // Configura la velocidad inicial del motor
}

void loop() {
    if (!client.connected()) {
        reconnectMQTT();
    }
    client.loop();
    moveMotor();  // Mueve el motor según la configuración actual
}

// Reconectar al servidor MQTT si la conexión se pierde
void reconnectMQTT() {
    while (!client.connected()) {
        Serial.println("Conectando a MQTT...");
        if (client.connect("ArduinoClient", mqttUser, mqttPassword)) {
            Serial.println("Conectado a MQTT");
            client.subscribe(mqttTopic);  // Se suscribe al tópico
        } else {
            Serial.print("Error de conexión. Intentando nuevamente en 5 segundos...");
            delay(5000);
        }
    }
}

// Función de callback para manejar mensajes MQTT
void mqttCallback(char* topic, byte* payload, unsigned int length) {
    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    Serial.print("Mensaje recibido: ");
    Serial.println(message);

    // Procesar el mensaje JSON
    if (topic == String(mqttTopic)) {
        // Aquí puedes hacer un parse del JSON si recibes algo como:
        // {"speed": 15, "steps": 1024}
        
        // Simplemente extraemos los valores que llegaron
        int newSpeed = extractValue(message, "speed");
        int newSteps = extractValue(message, "steps");

        // Actualizamos la configuración del motor si son válidos
        if (newSpeed != -1) motorSpeed = newSpeed;
        if (newSteps != -1) motorSteps = newSteps;

        stepper.setSpeed(motorSpeed);  // Establecer la nueva velocidad
        Serial.print("Nueva configuración - Velocidad: ");
        Serial.print(motorSpeed);
        Serial.print(" RPM, Pasos: ");
        Serial.println(motorSteps);
        
        // Enviar mensaje de confirmación a RabbitMQ
        client.publish("motor/status", "Listo");
    }
}

// Función para extraer valores de un JSON (simulada para simplificación)
int extractValue(String message, String key) {
    int startIndex = message.indexOf(key + "\":");
    if (startIndex == -1) return -1;
    startIndex += key.length() + 3;  // Para saltar la clave y el ":"
    int endIndex = message.indexOf(",", startIndex);
    if (endIndex == -1) endIndex = message.indexOf("}", startIndex);
    return message.substring(startIndex, endIndex).toInt();
}

// Función para mover el motor
void moveMotor() {
    // Mueve el motor de acuerdo a los pasos configurados
    stepper.step(motorSteps);  // Puede ser positivo o negativo para girar en ambos sentidos
    delay(500);  // Pausa entre movimientos
}

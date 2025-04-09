#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <AccelStepper.h>

// --- Configuración WiFi ---
const char *ssid = "arduino";
const char *password = "00000000";

// --- Configuración MQTT ---
const char *mqttServer = "189.244.34.160"; // IP de tu servidor MQTT
const int mqttPort = 1883;
const char *mqttUser = "sebas";
const char *mqttPassword = "sebas123";
const char *mqttTopic = "motor-limpieza";  // Tópico donde se recibirán los comandos de limpieza

WiFiClient espClient;
PubSubClient client(espClient);

// --- Pines del TB6600 ---
#define PUL 8    // Paso
#define DIR 7    // Dirección
#define ENA 6  // Pin para habilitar (Enable)

// --- Configuración del motor con AccelStepper ---
// Se usa el modo DRIVER (1 pulso y 1 dirección)
AccelStepper stepper(AccelStepper::DRIVER, PUL, DIR);

// Bandera para indicar que se está ejecutando una acción de limpieza
bool cleaningInProgress = false;

void initialPosition(){
  stepper.moveTo(-50);
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }
  
  // Activar el driver para retener la posición
  digitalWrite(ENA, LOW); // LOW activa el driver (torque activo)
  
  // Establecer la posición actual como referencia (0)
  stepper.setCurrentPosition(0);
  Serial.println("Motor listo en posición inicial (45° a la izquierda)");
}

void setup() {
  Serial.begin(115200);

  // Configurar el pin ENA y desactivar el torque al inicio
  pinMode(ENA, OUTPUT);
  digitalWrite(ENA, HIGH); // HIGH desactiva el driver en el TB6600
  
  delay(5000); // Esperar 5 segundos antes de moverse

  // Configurar parámetros del motor
  stepper.setMaxSpeed(50);
  stepper.setAcceleration(15);

  Serial.println("rellena el arenero");

  // Conexión a WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");

  // Configurar MQTT
  client.setServer(mqttServer, mqttPort);
  client.setCallback(mqttCallback);
  reconnectMQTT();
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  if (cleaningInProgress) {
    stepper.run();
  }
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.println("Conectando a MQTT...");
    if (client.connect("ArduinoClient", mqttUser, mqttPassword)) {
      Serial.println("Conectado a MQTT");
      client.subscribe(mqttTopic);
    } else {
      Serial.println("Fallo la conexión, reintentando en 5 segundos...");
      delay(5000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  
  // Si el mensaje está vacío, se ignora
  if (message.length() == 0) return;
  
  Serial.print("Mensaje recibido: ");
  Serial.println(message);

  if (String(topic) == mqttTopic) {
    if (message.indexOf("completa") >= 0) {
      publishStatus("Limpiando: limpieza completa");
      limpiarCompleta();
    } 
    if (message.indexOf("normal") >= 0) {
      publishStatus("Limpiando: limpieza normal");
      limpiarNormal();
    }
    if (message.indexOf("relleno") >= 0) {
      initialPosition();
    }
  }
}

void publishStatus(String status) {
  client.publish("motor/status", status.c_str());
}

void limpiarCompleta() {
  cleaningInProgress = true;
  long posInicial = stepper.currentPosition();
  stepper.moveTo(posInicial - 100);
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }
  stepper.moveTo(posInicial);
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }
  digitalWrite(ENA, HIGH); // HIGH desactiva el driver en el TB6600
  Serial.println("listo para rellenar");
  cleaningInProgress = false;
  publishStatus("Limpieza completa finalizada");
}

void limpiarNormal() {
  cleaningInProgress = true;
  long posInicial = stepper.currentPosition();
  stepper.moveTo(posInicial + 150);
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }
  stepper.moveTo(posInicial);
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }
  cleaningInProgress = false;
  publishStatus("Limpieza normal finalizada");
}

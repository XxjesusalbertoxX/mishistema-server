#include <WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include <DHT.h>

// Configuración WiFi y MQTT
const char* ssid = "arduino";
const char* password = "00000000";
const char* mqtt_server = "3.139.69.116";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Sensor ambiental
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Servo
Servo myservo;

// Sensor de agua
const int pinSensorAgua = A1;

// Semáforo
const int redPin = 6;
const int yellowPin = 7;
const int greenPin = 8;
volatile bool semaforoEncendido = false;
unsigned long lastChange = 0;
int currentState = 0;
const unsigned long stateDurations[] = {5000, 4000, 5000, 2000};

// Ultrasonido
const int trigPin = 9;
const int echoPin = 10;

// Temporizadores
unsigned long lastWifiCheck = 0;
unsigned long lastMqttCheck = 0;
unsigned long lastWaterRead = 0;
unsigned long lastGasRead = 0;
unsigned long lastAmbientRead = 0;
const unsigned long wifiInterval = 10000;
const unsigned long mqttInterval = 5000;
const unsigned long intervaloAgua = 10000;
const unsigned long intervaloGas = 10000;

// Configuración WiFi
void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
}

// Medir distancia con ultrasonido
float medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

// Mover servo
void moverServo(int pos) {
  myservo.write(pos);
}

// Callback para manejar mensajes MQTT
void sensorSettings(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  message.trim();

  if (String(topic) == "encender-semaforo") {
    semaforoEncendido = message.equalsIgnoreCase("on");
    if (semaforoEncendido) {
      currentState = 0;
      lastChange = millis();
    } else {
      apagarLeds();
    }
  } else if (String(topic) == "mover-servo") {
    moverServo(message.toInt());
  }
}

// Publicar datos de sensores
void publicarSensores() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastWaterRead >= intervaloAgua) {
    lastWaterRead = currentMillis;
    float distancia = medirDistancia();
    client.publish("sensor-distancia", String(distancia).c_str(), true);

    int nivelAgua = analogRead(pinSensorAgua);
    client.publish("sensor-nivelAgua", String(nivelAgua).c_str());
  }

  if (currentMillis - lastGasRead >= intervaloGas) {
    lastGasRead = currentMillis;
    int nivelGas = analogRead(A2); // Asumiendo que el sensor de gas está en A2
    client.publish("sensor-nivelGas", String(nivelGas).c_str());
  }

  if (currentMillis - lastAmbientRead >= intervaloGas) {
    lastAmbientRead = currentMillis;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (!isnan(h) && !isnan(t)) {
      String payload = "{ \"temperatura\": " + String(t) + ", \"humedad\": " + String(h) + " }";
      client.publish("sensor-ambiente", payload.c_str());
    }
  }
}

// Reconectar MQTT
void reconnect() {
  while (!client.connected()) {
    if (client.connect("MKR_SemaforoClient", "sebas", "sebas12345")) {
      client.subscribe("encender-semaforo");
      client.subscribe("mover-servo");
    } else {
      delay(5000);
    }
  }
}

// Apagar LEDs del semáforo
void apagarLeds() {
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);
}

// Actualizar semáforo
void actualizarSemaforo(int state) {
  apagarLeds();
  String color = "";

  switch (state) {
    case 0:
      digitalWrite(redPin, HIGH);
      color = "rojo";
      break;
    case 1:
      digitalWrite(yellowPin, HIGH);
      color = "amarillo";
      break;
    case 2:
      digitalWrite(greenPin, HIGH);
      color = "verde";
      break;
    case 3:
      digitalWrite(yellowPin, HIGH);
      color = "amarillo intermitente";
      break;
    default:
      color = "apagado";
      break;
  }

  client.publish("semaforo-color", color.c_str());
}

// Configuración inicial
void setup() {
  Serial.begin(115200);

  // Configuración de pines
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  apagarLeds();
  dht.begin();
  myservo.attach(3); // Asumiendo que el servo está conectado al pin 3

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(sensorSettings);
}

// Bucle principal
void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastWifiCheck >= wifiInterval) {
    lastWifiCheck = currentMillis;
    if (WiFi.status() != WL_CONNECTED) {
      setup_wifi();
    }
  }

  if (currentMillis - lastMqttCheck >= mqttInterval) {
    lastMqttCheck = currentMillis;
    if (!client.connected()) {
      reconnect();
    }
  }
  client.loop();

  if (semaforoEncendido && currentMillis - lastChange >= stateDurations[currentState]) {
    lastChange = currentMillis;
    currentState = (currentState + 1) % 4;
    actualizarSemaforo(currentState);
  }

  publicarSensores();
}


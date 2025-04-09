#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <AccelStepper.h>
#include <DHT.h>

// ----- Datos de red y MQTT (valores fijos) -----
const char* ssid = "arduino";
const char* password = "00000000";
const char* mqttServer = "www.atenasoficial.com";  // También puedes usar tu dominio, ej. "www.atenasoficial.com"
const int mqttPort = 1883;
const char* mqttUser = "sebas";
const char* mqttPassword = "sebas123";

// ----- Tópicos MQTT -----
const char* TOPIC_MOTOR_COMMAND     = "motor-limpieza";
const char* TOPIC_MOTOR_STATUS      = "motor-status";
const char* TOPIC_SENSOR_DHT        = "sensor-dht";
const char* TOPIC_SENSOR_MQ2        = "sensor-mq2";
const char* TOPIC_SENSOR_ULTRASONIC = "sensor-ultrasonic";
const char* TOPIC_SENSOR_CAT        = "sensor-cat";

// ----- Objetos MQTT -----
WiFiClient espClient;
PubSubClient client(espClient);

// ----- Configuración del Motor (TB6600) -----
#define PUL 8    // Pin de paso
#define DIR 7    // Pin de dirección
#define ENA 6    // Pin de habilitación (LOW activa torque)

// ----- Sensores -----
// DHT11
#define DHTPIN 3         
#define DHTTYPE DHT11
// MQ2 analógico
#define MQ2_ANALOG_PIN A0  
// Sensor ultrasónico para limpieza (HC-SR04)
#define trigPin 9        
#define echoPin 10       
// Sensor ultrasónico que verifica al gato en el arenero
#define CAT_TRIG_PIN 4   
#define CAT_ECHO_PIN 5

DHT dht(DHTPIN, DHTTYPE);
AccelStepper stepper(AccelStepper::DRIVER, PUL, DIR);

// ----- Intervalos de lectura (ms) -----
unsigned long lastUltrasonicRead = 0, lastDHTRead = 0, lastMQ2Read = 0, lastCatRead = 0;
unsigned long ultrasonicInterval = 2000;
unsigned long dhtInterval = 3000;
unsigned long mq2Interval = 2000;
unsigned long catInterval = 10000;  // Lectura cada 10 s

// ----- Umbrales -----
const int GAS_THRESHOLD = 500;
const float CAT_THRESHOLD = 25.0;
const float TEMP_THRESHOLD = 35.0; // 35°C

// ----- Variables para detección del gato -----
unsigned long catDetectionStart = 0;  // Para medir 5 s de detección continua
bool catInside = false;

// ----- Autómata del Motor -----
// Definimos el enum solo una vez
enum CleaningMode { NONE, COMPLETA, NORMAL, REPOSICION };
CleaningMode cleaningMode = NONE;
int cleaningStep = 0;
long initialMotorPosition = 0;
long targetPosition = 0;
CleaningMode lastCleaningCommand = NONE;

bool gasLock = false;
bool motorEnabled = false;

// ------------------ Función para reconectar MQTT ------------------
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.println("Conectando a MQTT...");
    if (client.connect("ArduinoClient", mqttUser, mqttPassword)) {
      Serial.println("Conectado a MQTT");
      client.subscribe(TOPIC_MOTOR_COMMAND);
    } else {
      Serial.println("Fallo en conexión MQTT, reintentando...");
      delay(5000);
    }
  }
}

// ------------------ Callback MQTT ------------------
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  if (message.length() == 0) return;
  Serial.print("Mensaje recibido: ");
  Serial.println(message);

  // Comando "relleno": activa torque, reposiciona (mueve -50 pasos) y habilita el motor.
  if (String(topic) == TOPIC_MOTOR_COMMAND) {
    if (message.indexOf("relleno") >= 0) {
      digitalWrite(ENA, LOW);  // Activa torque
      motorEnabled = true;
      stepper.setCurrentPosition(0);
      stepper.moveTo(-50);
      while (stepper.distanceToGo() != 0) {
        stepper.run();
      }
      stepper.setCurrentPosition(0);
      client.publish(TOPIC_MOTOR_STATUS, "Relleno: motor habilitado y reposicionado");
      return;
    }
    if (!motorEnabled) {
      Serial.println("Motor inhabilitado, espera 'relleno'");
      return;
    }
    if (message.indexOf("completa") >= 0) {
      client.publish(TOPIC_MOTOR_STATUS, "Limpiando: limpieza completa");
      cleaningMode = COMPLETA;
      cleaningStep = 0;
      initialMotorPosition = stepper.currentPosition();
      lastCleaningCommand = COMPLETA;
    } else if (message.indexOf("normal") >= 0) {
      client.publish(TOPIC_MOTOR_STATUS, "Limpiando: limpieza normal");
      cleaningMode = NORMAL;
      cleaningStep = 0;
      initialMotorPosition = stepper.currentPosition();
      lastCleaningCommand = NORMAL;
    }
  }
}

// ------------------ Función para publicar estado ------------------
void publishStatus(String status) {
  client.publish(TOPIC_MOTOR_STATUS, status.c_str());
}

// ------------------ Funciones de lectura de sensores ------------------

void readUltrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 20000);
  float distance = duration * 0.034 / 2;
  client.publish(TOPIC_SENSOR_ULTRASONIC, String(distance).c_str());
  Serial.print("Ultrasonico: ");
  Serial.print(distance);
  Serial.println(" cm");
}

void readDHT() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  String msg = (isnan(h) || isnan(t)) ? "Error" : "H:" + String(h) + "% T:" + String(t) + "C";
  client.publish(TOPIC_SENSOR_DHT, msg.c_str());
  Serial.println("DHT: " + msg);
  if (t > TEMP_THRESHOLD) {
    publishStatus("Alerta: Temperatura alta, caja necesita lavado");
  }
}

void readMQ2() {
  int val = analogRead(MQ2_ANALOG_PIN);
  client.publish(TOPIC_SENSOR_MQ2, String(val).c_str());
  Serial.print("MQ2: ");
  Serial.println(val);
  if (val >= GAS_THRESHOLD && !gasLock) {
    gasLock = true;
    publishStatus("Alerta: Gas excedido, motor bloqueado");
    cleaningMode = NONE;
  } else if (val < GAS_THRESHOLD && gasLock) {
    gasLock = false;
    publishStatus("Nivel de gas normal, motor habilitado");
  }
}

void readUltrasonicCat() {
  digitalWrite(CAT_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(CAT_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(CAT_TRIG_PIN, LOW);
  long d = pulseIn(CAT_ECHO_PIN, HIGH, 20000);
  float cm = d * 0.034 / 2.0;
  client.publish(TOPIC_SENSOR_CAT, String(cm).c_str());
  Serial.print("Sensor Cat: ");
  Serial.print(cm);
  Serial.println(" cm");
  
  // Detección continua: si se detecta al gato (distancia < CAT_THRESHOLD) durante 5 s, se bloquea el motor.
  if (cm < CAT_THRESHOLD) {
    if (catDetectionStart == 0) {
      catDetectionStart = millis();
    } else if (millis() - catDetectionStart >= 5000 && !catInside) {
      catInside = true;
      publishStatus("Alerta: Gato detectado en el arenero, motor bloqueado");
      Serial.println("Gato detectado en el arenero");
    }
  } else {
    catDetectionStart = 0;
    if (catInside) {
      catInside = false;
      publishStatus("Gato ausente, motor habilitado");
      Serial.println("Gato ausente");
    }
  }
}

// ------------------ Máquina de estados del motor ------------------
void cleaningStateMachine() {
  stepper.run();
  if (cleaningStep == 0) {
    if (cleaningMode == COMPLETA) {
      targetPosition = initialMotorPosition - 63;  // 63 pasos hacia atrás
      cleaningStep = 1;
    } else if (cleaningMode == NORMAL) {
      targetPosition = initialMotorPosition + 120; // 120 pasos hacia adelante
      cleaningStep = 1;
    } else if (cleaningMode == REPOSICION) {
      targetPosition = -58;  // 58 pasos para reposición
      cleaningStep = 1;
    }
    stepper.moveTo(targetPosition);
  } else if (cleaningStep == 1 && stepper.distanceToGo() == 0) {
    if (cleaningMode == REPOSICION) {
      publishStatus("Reposicion finalizada");
      cleaningMode = NONE;
      cleaningStep = 0;
      digitalWrite(ENA, HIGH);  // Desactiva torque
      motorEnabled = false;
    } else {
      stepper.moveTo(initialMotorPosition);
      cleaningStep = 2;
    }
  } else if (cleaningStep == 2 && stepper.distanceToGo() == 0) {
    if (lastCleaningCommand == COMPLETA) {
      publishStatus("Limpieza completa finalizada");
      digitalWrite(ENA, HIGH);  // Desactiva torque en COMPLETA
      motorEnabled = false;
    } else if (lastCleaningCommand == NORMAL) {
      publishStatus("Limpieza normal finalizada");
      // En NORMAL se conserva el torque
    }
    cleaningMode = NONE;
    cleaningStep = 0;
  }
}

// ------------------ Setup y Loop ------------------

void setup() {
  Serial.begin(115200);
  
  // Conectar a WiFi (valores fijos)
  Serial.print("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConectado a WiFi. IP:");
  Serial.println(WiFi.localIP());
  
  // Configurar MQTT
  client.setServer(mqttServer, mqttPort);
  client.setCallback(mqttCallback);
  reconnectMQTT();
  
  // Configurar pines del motor y sensores
  pinMode(ENA, OUTPUT);
  digitalWrite(ENA, HIGH); // Torque desactivado
  stepper.setMaxSpeed(60);
  stepper.setAcceleration(60);
  stepper.setCurrentPosition(0);
  
  dht.begin();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(MQ2_ANALOG_PIN, INPUT);
  pinMode(CAT_TRIG_PIN, OUTPUT);
  pinMode(CAT_ECHO_PIN, INPUT);
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
  
  unsigned long now = millis();
  static unsigned long lastUltrasonicRead = 0, lastDHTRead = 0, lastMQ2Read = 0, lastCatRead = 0;
  
  if (now - lastUltrasonicRead >= ultrasonicInterval) {
    lastUltrasonicRead = now;
    readUltrasonic();
  }
  if (now - lastDHTRead >= dhtInterval) {
    lastDHTRead = now;
    readDHT();
  }
  if (now - lastMQ2Read >= mq2Interval) {
    lastMQ2Read = now;
    readMQ2();
  }
  if (now - lastCatRead >= catInterval) {
    lastCatRead = now;
    readUltrasonicCat();
  }
  
  // Si no hay bloqueo por gas ni se detecta al gato, se ejecuta la máquina de estados del motor.
  if (!gasLock && !catInside && cleaningMode != NONE) {
    cleaningStateMachine();
  }
}
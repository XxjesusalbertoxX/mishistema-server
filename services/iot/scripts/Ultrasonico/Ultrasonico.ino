#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "arduino";
const char* password = "00000000";
const char* mqtt_server = "3.139.69.116"; 

const char* mqtt_user = "sebas";
const char* mqtt_password = "sebas12345";
const char* topic = "arenero-ultrasonic-data"; 

WiFiClient wifiClient;
PubSubClient client(wifiClient);

const int trigPin = 10;
const int echoPin = 13;

void setup_wifi() {
  Serial.print("Conectando a WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando al broker MQTTX...");
    if (client.connect("ESP32_Client", mqtt_user, mqtt_password)) {
      Serial.println("Conectado a MQTTX");
    } else {
      Serial.print("Falló, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

float medirDistancia() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2; 
  return distance;
}

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float distancia = medirDistancia();
  Serial.print("Distancia: ");
  Serial.println(distancia);

  char msg[10]; 
  sprintf(msg, "%.2f", distancia);
  client.publish(topic, msg, true);

  delay(1000);
}




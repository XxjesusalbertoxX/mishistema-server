#include <WiFi.h>
#include <PubSubClient.h>

// Credenciales WiFi y datos del Broker MQTT
const char* ssid = "arduino";
const char* password = "00000000";
const char* mqtt_server = "3.145.207.251"; // Reemplaza con la IP o dominio de tu broker

WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Pines para las luces del semáforo
const int redPin    = 5;   // LED rojo conectado al pin 5
const int yellowPin = 6;   // LED amarillo conectado al pin 6
const int greenPin  = 7;   // LED verde conectado al pin 7

// Estado del semáforo: true = encendido, false = apagado
volatile bool semaforoEncendido = false;

// Variables para el ciclo del semáforo
unsigned long lastChange = 0;
int currentState = 0;

// Duraciones en milisegundos para cada estado
const unsigned long stateDurations[] = {
  5000, // Estado 0: Solo rojo
  2000, // Estado 1: Rojo + amarillo
  5000, // Estado 2: Solo verde
  2000  // Estado 3: Solo amarillo
};

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido en el topic: ");
  Serial.println(topic);

  // Convertir payload a String
  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  message.trim();
  Serial.print("Payload: ");
  Serial.println(message);

  // Si el mensaje es "on" o "off" en el topic "encendido"
  if (String(topic) == "encendido") {
    if (message.equalsIgnoreCase("on")) {
      semaforoEncendido = true;
      Serial.println("Semáforo ENCENDIDO");
      // Reiniciar ciclo del semáforo
      currentState = 0;
      lastChange = millis();
    }
    else if (message.equalsIgnoreCase("off")) {
      semaforoEncendido = false;
      Serial.println("Semáforo APAGADO");
      apagarLeds();
    }
  }
}

void reconnect() {
  // Intentar reconectar hasta lograrlo
  while (!client.connected()) {
    Serial.print("Conectando al broker MQTT...");
    if (client.connect("MKR_SemaforoClient")) {
      Serial.println("Conectado");
      // Suscribirse al topic "encendido"
      client.subscribe("encendido");
    } else {
      Serial.print("Falló, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void apagarLeds() {
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, LOW);
}

void setup() {
  Serial.begin(115200);

  // Configurar pines de salida para LEDs
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  apagarLeds();

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Si el semáforo está encendido, manejar el ciclo de luces
  if (semaforoEncendido) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastChange >= stateDurations[currentState]) {
      lastChange = currentMillis;
      currentState = (currentState + 1) % 4; // 4 estados: 0,1,2,3
      actualizarSemaforo(currentState);
    }
  }
}

void actualizarSemaforo(int state) {
  // Apagar todas las luces antes de actualizar
  apagarLeds();

  switch (state) {
    case 0: // Solo rojo
      digitalWrite(redPin, HIGH);
      Serial.println("Estado 0: Rojo");
      break;
    case 1: // Rojo y amarillo
      digitalWrite(yellowPin, HIGH);
      Serial.println("Estado 1: Rojo + Amarillo");
      break;
    case 2: // Solo verde
      digitalWrite(greenPin, HIGH);
      Serial.println("Estado 2: Verde");
      break;
    case 3: // Solo amarillo
      digitalWrite(yellowPin, HIGH);
      Serial.println("Estado 3: Amarillo");
      break;
    default:
      apagarLeds();
      break;
  }
}



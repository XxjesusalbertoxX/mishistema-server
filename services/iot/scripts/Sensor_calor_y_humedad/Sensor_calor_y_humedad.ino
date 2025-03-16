#include <DHT.h>

// Definir el pin donde está conectado el sensor
#define DHTPIN 2       // Cambia al pin donde conectaste el sensor
#define DHTTYPE DHT11  // Especificamos que usamos un DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando el sensor RQ-S003 (DHT11)");
  dht.begin();
}

void loop() {
  delay(2000);  // Espera 2 segundos entre lecturas

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Error al leer el sensor");
    return;
  }

  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.println(" °C");
}


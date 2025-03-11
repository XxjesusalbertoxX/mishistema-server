#define MQ2_PIN A0  // Pin del sensor

void setup() {
  Serial.begin(115200); // Inicializa el monitor serie
}

void loop() {
  int lectura = analogRead(MQ2_PIN);  // Lee el valor analógico
  Serial.print("Valor MQ-2: ");
  Serial.println(lectura);

  delay(1000);  // Espera 1 segundo
}



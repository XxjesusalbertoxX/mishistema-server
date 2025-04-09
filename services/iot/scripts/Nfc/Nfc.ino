#define TRIG_PIN 13  // Pin de disparo (Trigger)
#define ECHO_PIN 12  // Pin de respuesta (Echo)

void setup() {
  Serial.begin(115200);  // Iniciar comunicación serial
  pinMode(TRIG_PIN, OUTPUT);  // Configurar pin TRIG como salida
  pinMode(ECHO_PIN, INPUT);   // Configurar pin ECHO como entrada
}

void loop() {
  // Enviar un pulso de 10 microsegundos al pin TRIG para activar el sensor
  digitalWrite(TRIG_PIN, LOW);  // Asegurarse de que el TRIG esté en bajo
  delayMicroseconds(2);          // Esperar 2 microsegundos
  digitalWrite(TRIG_PIN, HIGH);  // Enviar el pulso
  delayMicroseconds(10);         // Mantenerlo en alto por 10 microsegundos
  digitalWrite(TRIG_PIN, LOW);   // Apagar el TRIG

  // Medir el tiempo que tarda en regresar el eco
  long duration = pulseIn(ECHO_PIN, HIGH);  // Lee la duración del pulso del pin ECHO

  // Calcular la distancia en centímetros
  // La velocidad del sonido es aproximadamente 343 m/s (0.0343 cm/µs)
  long distance = duration * 0.0343 / 2;  // Dividir entre 2 porque la señal va y vuelve

  // Mostrar la distancia en el monitor serial
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);  // Esperar medio segundo antes de la siguiente medición
}

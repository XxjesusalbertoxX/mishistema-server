#define ULTRAWATER_TRIG_PIN 14
#define ULTRAWATER_ECHO_PIN 12

long duration;
float distance;

void setup() {
  Serial.begin(115200);
  pinMode(ULTRAWATER_TRIG_PIN, OUTPUT);
  pinMode(ULTRAWATER_ECHO_PIN, INPUT);
}

void loop() {
  // Limpia el pin TRIG
  digitalWrite(ULTRAWATER_TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Envia un pulso de 10 microsegundos
  digitalWrite(ULTRAWATER_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRAWATER_TRIG_PIN, LOW);

  // Lee el tiempo que tarda el eco en volver
  duration = pulseIn(ULTRAWATER_ECHO_PIN, HIGH);

  // Calcula la distancia (en centímetros)
  distance = duration * 0.034 / 2;

  // Imprime la distancia
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500);
}

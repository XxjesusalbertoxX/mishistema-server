#define trigPin 9        
#define echoPin 10       

long duration;
int distance;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // Enviar un pulso corto al trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Medir el tiempo que tarda en regresar el eco
  duration = pulseIn(echoPin, HIGH);

  // Calcular la distancia en centímetros
  distance = duration * 0.034 / 2;

  // Mostrar la distancia en el monitor serial
  Serial.print("Distancia: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500); // Esperar medio segundo antes de la siguiente medición
}

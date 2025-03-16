const int pinSensor = A1; 

void setup() {
  Serial.begin(115200); 
}

void loop() {
  int nivelAgua = analogRead(pinSensor); 

  Serial.print("Nivel de Agua: ");
  Serial.println(nivelAgua); 

  delay(1000);
}

#define MQ2_PIN A0  

void setup() {
  Serial.begin(115200); 
}

void loop() {
  int lectura = analogRead(MQ2_PIN);  
  Serial.print("Valor MQ-2: ");
  Serial.println(lectura);

  delay(1000); 
}



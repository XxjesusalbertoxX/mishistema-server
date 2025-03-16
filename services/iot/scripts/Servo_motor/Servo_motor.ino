#include <Servo.h>  // Incluir la librería para controlar servomotores

Servo myservo;  // Crear un objeto de tipo Servo

void setup() {
  myservo.attach(9);  // Conectar el servomotor al pin 9 (reemplaza "1" por "9" ya que el pin 1 está reservado para la comunicación serial en la mayoría de los Arduino)
  Serial.begin(9600);  // Iniciar la comunicación serial
}

void loop() {
  // Mover el servomotor de 0 a 90 grados
  for (int pos = 0; pos <= 90; pos++) {
    myservo.write(pos);  // Mover el servomotor a la posición "pos"
    delay(5);  // Esperar 30 ms para que el servomotor se mueva (más lento)
  }

  // Mover el servomotor de 90 a 0 grados
  for (int pos = 90; pos >= 0; pos--) {
    myservo.write(pos);  // Mover el servomotor a la posición "pos"
    delay(5);  // Esperar 30 ms para que el servomotor se mueva (más lento)
  }
}

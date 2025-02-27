#include <Stepper.h>

// Definir el número de pasos por revolución
#define STEPS_PER_REV 2048  // 512 pasos = 360° para el 28BYJ-48

// Configurar los pines de conexión (en orden de IN1 a IN4 del driver ULN2003)
Stepper stepper(STEPS_PER_REV, 4, 6, 5, 7);

void setup() {
    stepper.setSpeed(10);  // Velocidad en RPM (puedes ajustarlo)
    Serial.begin(115200);
}

void loop() {
    Serial.println("Girando en sentido horario...");
    stepper.step(STEPS_PER_REV);  // Una vuelta completa en sentido horario
    delay(2000);

    Serial.println("Girando en sentido antihorario...");
    stepper.step(-STEPS_PER_REV); // Una vuelta completa en sentido antihorario
    delay(2000);
}

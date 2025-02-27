#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

float gyroX_offset = 0;
float gyroY_offset = 0;
int numSamples = 100;  // Número de muestras para la calibración

void setup() {
  Serial.begin(115200);
  // Para MKR WiFi 1010: SDA en Pin 11, SCL en Pin 12
  Wire.begin(11, 12);  
  
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("Error: No se pudo conectar al MPU6050");
    while (1);
  }
  
  Serial.println("Calibrando giroscopio...");
  for (int i = 0; i < numSamples; i++) {
    int16_t gx, gy, gz;
    mpu.getRotation(&gx, &gy, &gz);
    gyroX_offset += gx;
    gyroY_offset += gy;
    delay(10);
  }
  gyroX_offset /= numSamples;
  gyroY_offset /= numSamples;
  Serial.println("Calibración completada");
}

void loop() {
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  // Convertir los valores del giroscopio a grados/segundo y aplicar la calibración
  float gyroX = (gx - gyroX_offset) / 131.0;
  float gyroY = (gy - gyroY_offset) / 131.0;
  
  Serial.print("Gyro X: ");
  Serial.print(gyroX);
  Serial.print(" | Gyro Y: ");
  Serial.println(gyroY);
  
  delay(500);
}

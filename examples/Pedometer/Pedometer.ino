#include <Wire.h>
#include <MannaNAUTA_LSM6DS.h>

MannaNAUTA_LSM6DS imu;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("mannaNAUTA LSM6DS - pedometer");

  if (!imu.begin()) {
    Serial.println("Falha ao encontrar o LSM6DS.");
    while (1) {
      delay(10);
    }
  }

  Serial.print("Sensor detectado: ");
  Serial.println(imu.variantName());

   // acelerometro em 2G e taxa de 26 Hz.
  imu.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  imu.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
  imu.setAccelDataRate(LSM6DS_RATE_26_HZ);
  imu.setGyroDataRate(LSM6DS_RATE_26_HZ);

  // Saida de deteccao de passo no INT1, se o pino estiver ligado no hardware.
  imu.configInt1(false, false, false, true);

  imu.resetPedometer();
  imu.enablePedometer(true);

  Serial.println("Pedometer habilitado.");
}

void loop() {
  Serial.print("Passos: ");
  Serial.println(imu.readPedometer());

  delay(100);
}
